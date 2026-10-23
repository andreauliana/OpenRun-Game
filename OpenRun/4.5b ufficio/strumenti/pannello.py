#!/usr/bin/env python3
"""Pannello di progresso (sola lettura). Uso: python3 strumenti/pannello.py [--porta 8088] [--solo-locale]
Il controller V4 usa la porta 8090; avvio manuale predefinito 8088. Solo libreria standard."""
import json, os, re, subprocess, time, argparse
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

BASE = os.environ.get("V3_BASE") or os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
BASE = os.path.abspath(BASE)
S = os.path.join(BASE, "sglavinated_src")
AGENTI = ["capo","developer","developer-junior","grafico"]  # ruoli 4.6 Studio
ANSI = re.compile(r"\x1b\[[0-9;]*[A-Za-z]")
DATA_LOG = re.compile(r"(?<!\d)(\d{1,2})/\d{1,2}(?:/\d{2,4})?(\s+\d{2}:\d{2})(?!\d)")

def data_pannello(t):
    return DATA_LOG.sub(r"\1\2", t)

def leggi(p, n=None):
    try:
        with open(os.path.join(S, p), encoding="utf-8", errors="replace") as f:
            t = f.read()
        return t if n is None else "\n".join(t.splitlines()[-n:])
    except OSError:
        return ""

def conta(d):
    n, ultimo = 0, 0
    for r, _, fs in os.walk(os.path.join(S, d)):
        for f in fs:
            if f == "LEGGIMI.md": continue
            n += 1
            try: ultimo = max(ultimo, os.path.getmtime(os.path.join(r, f)))
            except OSError: pass
    return n, ultimo

def processi():
    try:
        pid = int(leggi(".ciclo.pid").strip())
        cmd = subprocess.run(["ps", "-p", str(pid), "-o", "command="], capture_output=True, text=True, timeout=5).stdout
    except ValueError:
        return {"opencode": 0, "ciclo": 0, "in_corso": []}
    except (OSError, subprocess.TimeoutExpired):
        return {"opencode": -1, "ciclo": -1, "in_corso": []}
    if os.path.join(BASE, "strumenti", "ciclo.sh") not in cmd:
        return {"opencode": 0, "ciclo": 0, "in_corso": []}
    try:
        agente, avvio = leggi(".agente_corrente").split()
        if agente not in AGENTI: raise ValueError
        secondi = max(0, int(time.time()) - int(avvio))
    except (ValueError, TypeError):
        return {"opencode": 0, "ciclo": 1, "in_corso": []}
    return {"opencode": 1, "ciclo": 1, "in_corso": [{"nome": agente, "sec": secondi}]}

def blocchi():
    out, cur = [], None
    for l in leggi("RICHIESTA.md").splitlines():
        m = re.match(r"## BLOCCO (\S+)\s+[—-]\s+(.*)", l)
        if m: cur = {"id": m.group(1), "agente": " ".join(re.findall(r"@([\w-]+)", m.group(2).split("(")[0])), "stato": "?"}; out.append(cur); continue
        m = re.match(r"\|\s*Stato\s*\|\s*(\[.\])", l)
        if m and cur: cur["stato"] = m.group(1)
    return [b for b in out if b["id"] not in ("0", "X")]

SALTA = {".opencode", ".registro_bar", "outrun_src", "node_modules", ".git", "archivio_per_claude"}
def recenti(n=16, finestra=7200, bin_s=600):
    adesso = time.time(); lista = []; bins = [0] * (finestra // bin_s)
    for r, ds, fs in os.walk(S):
        ds[:] = [d for d in ds if d not in SALTA]
        for f in fs:
            if f in ("notte.log", ".marker_giro", "giro.log") or f.endswith(".tmp"): continue
            fp = os.path.join(r, f)
            try: mt = os.path.getmtime(fp)
            except OSError: continue
            if adesso - mt > finestra: continue
            rel = os.path.relpath(fp, S)
            bins[min(len(bins) - 1, int((adesso - mt) // bin_s))] += 1
            lista.append((mt, rel))
    lista.sort(reverse=True)
    out = []
    for mt, rel in lista[:n]:
        parti = rel.split(os.sep); ag = parti[1] if parts_ok(parti) else ("grafico" if parti[:2] == ["UFFICIO", "Dipartimento Grafico"] else "")
        out.append({"rel": rel, "sec": int(adesso - mt), "agente": ag})
    return out, bins[::-1]

def parts_ok(p):
    return len(p) > 2 and p[0] in ("bacheche", "out", "log_agenti") and p[1] in AGENTI

def attivi():
    t = leggi(".agenti_attivi").split()
    return t or ["developer", "developer-junior"]

def versione():
    v = leggi(".versione").strip()
    fb = leggi("ANDREA_REVIEW.md")
    m = re.search(r"Tappa attiva:\s*(\S+)", fb)
    d = os.path.join(S, "out", "review")
    rv = sorted(os.listdir(d), key=lambda f: int(re.sub(r"\D", "", f) or 0)) if os.path.isdir(d) else []
    return {"versione": "v" + v if v else "", "tappa": m.group(1) if m else "?", "ultima_review": rv[-1] if rv else ""}

DOCS = {"richiesta": "sglavinated_src/RICHIESTA_APERTA.md", "feedback": "sglavinated_src/ANDREA_REVIEW.md",
        "stato": "sglavinated_src/Checkpoint Revision/STATO_CORRENTE.md",
        "officina": "sglavinated_src/UFFICIO/Officina/STATUS_OFFICINA.md", "tappe": "sglavinated_src/TAPPE.md"}
def documento(k):
    p = os.path.join(BASE, DOCS.get(k, DOCS["richiesta"]))
    try: return open(p, encoding="utf-8", errors="replace").read()
    except OSError: return "(file non ancora presente)"

def log_agente(a):
    if a not in AGENTI and a != "ciclo": return ""
    if a == "ciclo": return data_pannello(ANSI.sub("", leggi("notte.log", 120)))
    return data_pannello(ANSI.sub("", leggi(f"log_agenti/{a}.log", 120)))

def modello(a):
    t = leggi(f".opencode/agents/{a}.md")
    m = re.search(r"^model:\s*(\S+)", t, re.M)
    return m.group(1).replace("opencode/", "").replace("-free", "") if m else "?"

def stato():
    adesso = time.time(); pr = processi(); inc = {x["nome"]: x["sec"] for x in pr["in_corso"]}
    ag = []; squadra = attivi()
    for a in AGENTI:
        n1, t1 = conta(f"bacheche/{a}"); n2, t2 = conta(f"out/{a}")
        try: t3 = os.path.getmtime(os.path.join(S, "log_agenti", a + ".log"))
        except OSError: t3 = 0
        t = max(t1, t2, t3)
        mem = leggi(f"bacheche/{a}/RIASSUNTO.md")
        try: tm = int(adesso - os.path.getmtime(os.path.join(S, "bacheche", a, "RIASSUNTO.md")))
        except OSError: tm = None
        ag.append({"nome": a, "file": n1 + n2, "modello": modello(a), "in_corso": a in inc, "sec_corso": inc.get(a), "sec_ultimo": int(adesso - t) if t else None,
                   "attivo": a in squadra, "ruolo": "squadra" if a in squadra else ("coordina" if a == "capo" else "spento"),
                   "memoria": "\n".join(mem.splitlines()[:12]), "memoria_sec": tm})
    nout, _ = conta("out"); nlib, _ = conta(os.path.join("UFFICIO", "Dipartimento Grafico", "Libreria"))
    log_pulito = ANSI.sub("", leggi("notte.log", 400)); righe = log_pulito.splitlines()
    ciclo = [data_pannello(l) for l in righe if l.startswith("[ciclo]")][-14:]
    try: eta = int(adesso - os.path.getmtime(os.path.join(S, "notte.log")))
    except OSError: eta = None
    att, bins = recenti()
    ultima_modifica = att[0]["sec"] if att else None
    ultimo_ciclo = ciclo[-1] if ciclo else ""
    pausa = ("LIMITE DI RICHIESTE" in ultimo_ciclo) or ("Rate limit" in "\n".join(righe[-12:]))
    review = "pronta per la review" in ultimo_ciclo
    if pr["in_corso"]: gen = "lavora"
    elif ultima_modifica is not None and ultima_modifica < 300: gen = "attivo"
    elif pr["ciclo"] > 0 and pausa: gen = "pausa"
    elif pr["ciclo"] > 0: gen = "attesa"
    elif review: gen = "review"
    else: gen = "fermo"
    return {"ora": time.strftime("%a %d %H:%M"), "generale": gen, "processi": pr, "log_sec": eta, "ultima_modifica_sec": ultima_modifica,
            "ciclo": ciclo, "log": "\n".join(righe[-25:]), "attivita": att, "bins": bins,
            "agenti": ag, "blocchi": blocchi(), "out": nout, "libreria": nlib, "squadra": squadra, "ver": versione(),
            "decisioni_aperte": len(re.findall(r"^## (?:D\d+|A\d+|T1-Q\d+)[^\n]*\[APERTA\]", leggi("ANDREA_REVIEW.md"), re.M))}

PAGINA = r"""<!doctype html><html lang=it><meta charset=utf-8><meta name=viewport content="width=device-width,initial-scale=1">
<title>Pannello OpenRun 4.6</title><style>
:root{--bg:#0f1115;--fg:#e6e6e6;--mut:#8a93a2;--ok:#4caf7d;--wait:#e0a93b;--bad:#e5534b;--card:#181b21;--line:#262b33}
@media(prefers-color-scheme:light){:root{--bg:#f5f6f8;--fg:#1c1f26;--mut:#5b6472;--card:#fff;--line:#e3e6ea}}
*{box-sizing:border-box}body{margin:0;padding:16px;background:var(--bg);color:var(--fg);font:14px/1.45 system-ui,sans-serif}
h1{font-size:18px;margin:0 0 12px;display:flex;gap:10px;align-items:center;flex-wrap:wrap}
h2{font-size:12px;color:var(--mut);text-transform:uppercase;letter-spacing:.06em;margin:0 0 8px}
.g{display:grid;gap:12px;grid-template-columns:repeat(auto-fit,minmax(min(310px,100%),1fr))}.tw{overflow-x:auto}
.tabs{display:flex;flex-wrap:wrap;gap:6px;margin-bottom:8px}.tabs button{background:var(--line);color:var(--fg);border:0;border-radius:99px;padding:3px 10px;font:12px system-ui;cursor:pointer}.tabs button.on{background:var(--ok);color:#fff}
details{border-bottom:1px solid var(--line);padding:4px 0}summary{cursor:pointer}details pre{max-height:180px;margin-top:4px}
.md{max-height:520px;overflow:auto}.md table{font-size:13px}.md h1,.md h2,.md h3{color:var(--fg);text-transform:none;letter-spacing:0;font-size:15px;margin:12px 0 6px}.md code{font:12px ui-monospace,monospace;background:var(--line);padding:0 4px;border-radius:4px}.md ul{margin:4px 0;padding-left:20px}
.big.review{background:#5b8def;animation:pulse2 2.4s infinite}tr.spento td{opacity:.45}.c{background:var(--card);border-radius:10px;padding:12px;overflow:hidden}
.w{grid-column:1/-1}
table{width:100%;border-collapse:collapse}td,th{padding:4px 6px;text-align:left;border-bottom:1px solid var(--line)}th{color:var(--mut);font-weight:500}tr:last-child td{border:0}
pre{margin:0;white-space:pre-wrap;word-break:break-word;font:12px/1.4 ui-monospace,monospace;max-height:300px;overflow:auto}
.b{display:inline-block;padding:1px 9px;border-radius:99px;font-size:12px}.ok{background:#4caf7d33;color:var(--ok)}.wait{background:#e0a93b33;color:var(--wait)}.bad{background:#e5534b33;color:var(--bad)}.mut{color:var(--mut)}
.dot{display:inline-block;width:10px;height:10px;border-radius:50%;background:#6b7280;margin-right:8px;vertical-align:middle}
.dot.on{background:var(--ok);animation:pulse 1.2s infinite}.dot.rec{background:var(--wait)}
@keyframes pulse{0%{box-shadow:0 0 0 0 #4caf7daa}70%{box-shadow:0 0 0 10px #4caf7d00}100%{box-shadow:0 0 0 0 #4caf7d00}}
.hero{display:flex;gap:14px;align-items:center}.big{width:18px;height:18px;border-radius:50%;background:#6b7280;flex:none}
.big.lavora,.big.attivo{background:var(--ok);animation:pulse 1.2s infinite}.big.pausa,.big.attesa{background:var(--wait);animation:pulse2 2.4s infinite}.big.fermo{background:var(--bad)}
@keyframes pulse2{0%,100%{opacity:1}50%{opacity:.35}}
.bar{height:10px;border-radius:99px;background:var(--line);overflow:hidden;margin:6px 0}.bar i{display:block;height:100%;background:linear-gradient(90deg,var(--ok),#7ee0a8);background-size:40px 100%;animation:slide 1.4s linear infinite;transition:width .6s}
@keyframes slide{from{background-position:0 0}to{background-position:40px 0}}
.feed div{padding:3px 0;border-bottom:1px solid var(--line);font:12px ui-monospace,monospace;display:flex;gap:8px}.feed div.nuovo{animation:flash 2.5s}
@keyframes flash{from{background:#4caf7d44}to{background:transparent}}
.feed .t{color:var(--mut);min-width:64px}.feed .a{color:var(--wait);min-width:76px}
svg{width:100%;height:60px;display:block}.run{animation:pulse2 1.1s infinite}
</style>
<h1>OpenRun 4.6 Studio · pannello <span id=ver class="b ok"></span> <span id=ora class=mut></span></h1>
<div class=g><div class="c w"><div class=hero><div class=big id=big></div><div><div id=titolo style="font-size:20px;font-weight:600">...</div><div id=sotto class=mut></div></div></div>
<div class=bar><i id=prog style="width:0"></i></div><div id=progt class=mut></div></div>
<div class=c><h2>Blocchi aperti</h2><div class=tw><table id=bl></table></div></div>
<div class=c><h2>Agenti</h2><div class=tw><table id=ag></table></div></div>
<div class=c><h2>Attivit&agrave; file (ultime 2 ore, ogni 10 min)</h2><svg id=spark viewBox="0 0 240 60" preserveAspectRatio=none></svg><div class="feed" id=feed style="margin-top:8px"></div></div>
<div class=c><h2>Cosa fa il ciclo</h2><pre id=ciclo></pre></div>
<div class=c><h2>Log</h2><div class=tabs id=tabs></div><pre id=log></pre></div>
<div class=c><h2>Cosa pensa ogni agente (RIASSUNTO.md)</h2><div id=mem></div></div>
<div class="c w"><h2>Documenti (sola lettura)</h2><div class=tabs id=dtabs></div><div id=doc class=md></div></div></div>
<script>
const e=s=>String(s).replace(/[&<>]/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;'}[c]));
const fa=s=>s==null?'-':s<60?s+' s':s<3600?Math.floor(s/60)+' min':Math.floor(s/3600)+' h '+Math.floor(s%3600/60)+' min';
const T={lavora:'Stanno lavorando',attivo:'Attivo (file scritti da poco)',pausa:'In pausa (limite di richieste)',attesa:'Ciclo in attesa',review:'Versione pronta: aspetta il tuo feedback',fermo:'Fermo'};
let logAg='ciclo',chiusi=new Set();
const formatoOra=new Intl.DateTimeFormat('en-GB',{weekday:'short',day:'2-digit',hour:'2-digit',minute:'2-digit',second:'2-digit',hourCycle:'h23',timeZoneName:'short'});
function aggiornaOra(){const parti=Object.fromEntries(formatoOra.formatToParts(new Date()).map(p=>[p.type,p.value]));document.getElementById('ora').textContent=`${parti.weekday} ${parti.day} ${parti.hour}:${parti.minute}:${parti.second} ${parti.timeZoneName}`}
aggiornaOra();setInterval(aggiornaOra,1000);
async function carLog(){try{const tx=await (await fetch('/log?ag='+logAg)).text();const lg=document.getElementById('log'),bas=lg.scrollTop+lg.clientHeight>=lg.scrollHeight-5;lg.textContent=tx||'(log vuoto)';if(bas)lg.scrollTop=lg.scrollHeight}catch(x){}}
let tf=Date.now(),visti=new Set(),primo=true,ultimoOk=0;
async function t(){try{const d=await (await fetch('/api')).json();tf=Date.now();ultimoOk=tf;
document.getElementById('ver').textContent=[d.ver.versione,'tappa '+d.ver.tappa,d.ver.ultima_review?'review '+d.ver.ultima_review:''].filter(Boolean).join(' · ');
document.getElementById('big').className='big '+d.generale;
const n=d.processi.in_corso.map(x=>x.nome);
document.getElementById('titolo').textContent=T[d.generale]+(n.length?': '+n.join(', '):'');
document.title=(d.generale=='lavora'||d.generale=='attivo'?'● ':'○ ')+'OpenRun 4.6 '+d.generale;
document.getElementById('sotto').textContent='ultimo file scritto '+fa(d.ultima_modifica_sec)+' fa · log aggiornato '+fa(d.log_sec)+' fa · opencode '+d.processi.opencode+' · ciclo '+d.processi.ciclo+' · decisioni aperte '+d.decisioni_aperte;
const done=d.blocchi.filter(b=>b.stato==='[x]').length,tot=d.blocchi.length||1;
document.getElementById('prog').style.width=(100*done/tot)+'%';
document.getElementById('progt').textContent='Blocchi '+done+'/'+d.blocchi.length+' · out/ '+d.out+' file · libreria/ '+d.libreria;
const ord=[...d.agenti].sort((x,y)=>(y.ruolo!='spento')-(x.ruolo!='spento'));
document.getElementById('ag').innerHTML='<tr><th>agente<th>modello<th>file<th>stato</tr>'+ord.map(a=>{
 const rec=a.sec_ultimo!=null&&a.sec_ultimo<900;
 const st=a.in_corso?`<span class=run style="color:var(--ok)">in corso <span data-s="${a.sec_corso}" class=tk>${fa(a.sec_corso)}</span></span>`:`<span class=mut>${a.sec_ultimo==null?'mai':fa(a.sec_ultimo)+' fa'}</span>`;
 return `<tr class="${a.ruolo=='spento'?'spento':''}"><td><span class="dot ${a.in_corso?'on':rec?'rec':''}"></span>${a.nome}${a.ruolo=='squadra'?'':' <span class=mut>('+a.ruolo+')</span>'}</td><td class=mut>${e(a.modello)}</td><td>${a.file}</td><td>${st}</td></tr>`}).join('');
const f=document.getElementById('feed');
f.innerHTML=d.attivita.map(x=>{const k=x.rel+x.sec;const nuovo=!primo&&x.sec<12&&!visti.has(x.rel);visti.add(x.rel);return `<div class="${nuovo?'nuovo':''}"><span class=t>${fa(x.sec)}</span><span class=a>${e(x.agente)}</span><span>${e(x.rel)}</span></div>`}).join('')||'<span class=mut>nessun file modificato nelle ultime 2 ore</span>';
primo=false;
const b=d.bins,mx=Math.max(1,...b),w=240/b.length;
document.getElementById('spark').innerHTML=b.map((v,i)=>`<rect x="${i*w+1}" width="${w-2}" y="${60-55*v/mx}" height="${55*v/mx||1}" rx="2" fill="${i==b.length-1?'#4caf7d':'#4caf7d88'}"/>`).join('');
document.getElementById('ciclo').textContent=d.ciclo.join('\n');
const aperti_b=d.blocchi.filter(b=>b.stato!=='[x]'&&b.stato!=='[-]');
document.getElementById('bl').innerHTML=`<tr><th colspan=3>${aperti_b.length} aperti · ${done} chiusi</tr>`+aperti_b.map(b=>`<tr><td>${e(b.id)}</td><td class=mut>${e(b.agente)}</td><td><span class="b ${b.stato==='[x]'?'ok':b.stato==='[~]'?'wait':'mut'}">${e(b.stato)}</span></td></tr>`).join('');
const tb=['ciclo',...d.squadra,'capo'];
document.getElementById('tabs').innerHTML=tb.map(x=>`<button class="${x==logAg?'on':''}" onclick="logAg='${x}';carLog();document.querySelectorAll('#tabs button').forEach(b=>b.classList.toggle('on',b.textContent=='${x}'))">${x}</button>`).join('');
carLog();
document.getElementById('mem').innerHTML=d.agenti.filter(a=>a.ruolo=='squadra').map(a=>`<details data-a="${a.nome}" ${chiusi.has(a.nome)?'':'open'} ontoggle="this.open?chiusi.delete('${a.nome}'):chiusi.add('${a.nome}')"><summary>${a.nome} <span class=mut>${a.memoria_sec==null?'nessuna memoria':'aggiornata '+fa(a.memoria_sec)+' fa'}</span></summary><pre>${e(a.memoria||'(vuota: la scrive al prossimo turno)')}</pre></details>`).join('');
}catch(x){document.getElementById('titolo').textContent='Pannello non raggiungibile';document.getElementById('big').className='big fermo'}}
setInterval(()=>{document.querySelectorAll('.tk').forEach(el=>{el.textContent=fa(+el.dataset.s+Math.floor((Date.now()-tf)/1000))})},10000);
const DOCN={richiesta:'Blocchi aperti',feedback:'Il mio feedback',stato:'Stato corrente',officina:'Status Officina',tappe:'Tappe'};let docK='richiesta';
function inl(x){return e(x).replace(/\*\*(.+?)\*\*/g,'<b>$1</b>').replace(/`([^`]+)`/g,'<code>$1</code>')}
function md(t){const L=t.split('\n');let h='',i=0;while(i<L.length){const l=L[i];
 if(/^\|/.test(l)){const rows=[];while(i<L.length&&/^\|/.test(L[i])){if(!/^\|[\s|:-]+\|$/.test(L[i]))rows.push(L[i].replace(/^\||\|$/g,'').split('|'));i++}
  h+='<div class=tw><table>'+rows.map((r,k)=>'<tr>'+r.map(c=>(k?'<td>':'<th>')+inl(c.trim())).join('')+'</tr>').join('')+'</table></div>';continue}
 if(/^```/.test(l)){i++;let c='';while(i<L.length&&!/^```/.test(L[i]))c+=L[i++]+'\n';i++;h+='<pre>'+e(c)+'</pre>';continue}
 const m=l.match(/^(#{1,3})\s+(.*)/);if(m){h+='<h3>'+inl(m[2])+'</h3>';i++;continue}
 if(/^\s*([-*]|\d+\.)\s/.test(l)){h+='<ul>';while(i<L.length&&/^\s*([-*]|\d+\.)\s/.test(L[i]))h+='<li>'+inl(L[i++].replace(/^\s*([-*]|\d+\.)\s/,''))+'</li>';h+='</ul>';continue}
 h+=l.trim()?'<p>'+inl(l)+'</p>':'';i++}return h}
async function carDoc(){try{document.getElementById('doc').innerHTML=md(await (await fetch('/doc?f='+docK)).text())}catch(x){}
 document.getElementById('dtabs').innerHTML=Object.keys(DOCN).map(k=>`<button class="${k==docK?'on':''}" onclick="docK='${k}';carDoc()">${DOCN[k]}</button>`).join('')}
carDoc();setInterval(carDoc,30000);
t();setInterval(t,15000);
</script></html>"""

class H(BaseHTTPRequestHandler):
    def log_message(self, *a): pass
    def do_GET(self):
        if self.path.startswith("/api"): body, tipo = json.dumps(stato()).encode(), "application/json"
        elif self.path.startswith("/doc"):
            from urllib.parse import urlparse, parse_qs
            body, tipo = documento(parse_qs(urlparse(self.path).query).get("f", ["tappe"])[0]).encode(), "text/plain; charset=utf-8"
        elif self.path.startswith("/log"):
            from urllib.parse import urlparse, parse_qs
            a = parse_qs(urlparse(self.path).query).get("ag", ["ciclo"])[0]
            body, tipo = log_agente(a).encode(), "text/plain; charset=utf-8"
        elif self.path in ("/", "/index.html"): body, tipo = PAGINA.encode(), "text/html; charset=utf-8"
        else: self.send_error(404); return
        self.send_response(200); self.send_header("Content-Type", tipo); self.send_header("Cache-Control", "no-store"); self.end_headers(); self.wfile.write(body)

if __name__ == "__main__":
    ap = argparse.ArgumentParser(); ap.add_argument("--porta", type=int, default=8088); ap.add_argument("--solo-locale", action="store_true")
    a = ap.parse_args(); host = "127.0.0.1" if a.solo_locale else "0.0.0.0"
    print(f"[pannello] http://{host}:{a.porta}  (sola lettura, base: {BASE})"); ThreadingHTTPServer((host, a.porta), H).serve_forever()
