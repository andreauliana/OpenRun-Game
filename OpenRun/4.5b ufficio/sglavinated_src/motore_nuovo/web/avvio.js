// Open Run nel browser: avvio del gioco nel blocco #openrun-box.
// Prima di questo file la pagina imposta window.OPENRUN_BASE (cartella openrun/ con
// openrun.js, .wasm, .data e font) e window.OPENRUN_HOME (dove porta EXIT del menu).
(function () {
  var BASE = window.OPENRUN_BASE || "/openrun/";
  var HOME = window.OPENRUN_HOME || "/";
  var box = document.getElementById("openrun-box");
  if (!box) return;
  // Telefono e tablet: il gioco non parte (serve una tastiera), resta il bottone home.
  var soloTouch = window.matchMedia && window.matchMedia("(hover: none) and (pointer: coarse)").matches;
  var canvas = document.getElementById("canvas");
  var avvio = box.querySelector(".or-avvio");
  var messaggio = box.querySelector(".or-messaggio");
  var partito = false;

  try {
    var font = new FontFace("Born2bSporty", "url(" + BASE + "Born2bSportyV2.ttf)");
    font.load().then(function (f) { document.fonts.add(f); }, function () {});
  } catch (e) {}
  box.querySelectorAll("a.or-casa").forEach(function (a) { a.href = HOME; });

  window.Module = {
    canvas: canvas,
    locateFile: function (nome) { return BASE + nome; },
    print: function () {},
    printErr: function () {},
    setStatus: function (t) {
      var m = /\((\d+(?:\.\d+)?)\/(\d+)\)/.exec(t || "");
      if (m) messaggio.textContent = "LOADING " + Math.floor(100 * m[1] / m[2]) + "%";
    },
    onRuntimeInitialized: function () {
      avvio.classList.add("or-nascosto");
      canvas.focus({ preventScroll: true });
      // Il gioco si porta tutto in vista, sotto l'eventuale menu fisso del sito.
      var spazio = parseFloat(getComputedStyle(box).getPropertyValue("--or-spazio")) || 0;
      var su = box.getBoundingClientRect().top + window.scrollY - spazio;
      window.scrollTo({ top: Math.max(0, su), behavior: "smooth" });
    },
    sglEsci: function () { window.location.href = HOME; } // voce EXIT del menu
  };

  function parti() {
    if (partito || soloTouch) return;
    partito = true;
    messaggio.classList.remove("or-via");
    messaggio.textContent = "LOADING";
    var s = document.createElement("script");
    s.src = BASE + "openrun.js";
    s.onerror = function () { messaggio.textContent = "COULD NOT LOAD THE GAME"; };
    document.body.appendChild(s);
  }
  // L'audio del browser parte solo dopo un clic o un tasto: il gioco si carica li'.
  avvio.addEventListener("click", parti);
  window.addEventListener("keydown", function (e) {
    if (partito || e.ctrlKey || e.metaKey || e.altKey || e.key === "Tab") return;
    var t = e.target && e.target.tagName;
    if (t === "INPUT" || t === "TEXTAREA") return;
    e.preventDefault();
    parti();
  });
  window.addEventListener("gamepadconnected", function () { if (!partito) messaggio.textContent = "CLICK TO START"; });

  var intero = box.querySelector(".or-intero");
  if (intero) intero.addEventListener("click", function () {
    if (document.fullscreenElement) document.exitFullscreen();
    else if (box.requestFullscreen) box.requestFullscreen();
    else if (box.webkitRequestFullscreen) box.webkitRequestFullscreen();
    parti();
    canvas.focus({ preventScroll: true });
  });

  // Classifica online (solo se la pagina da' OPENRUN_API): i punteggi nuovi della
  // classifica locale vanno al sito, uno alla volta. La classifica del gioco resta locale.
  function inviaPunteggi() {
    if (!window.OPENRUN_API || !window.Module || !Module.FS) return;
    var g;
    try { g = JSON.parse(Module.FS.readFile("/sglavinated_src/salvataggi/giocatore.json", { encoding: "utf8" })); } catch (e) { return; }
    var inviati = {};
    try { inviati = JSON.parse(localStorage.getItem("openrun_inviati") || "{}"); } catch (e) {}
    var voci = [];
    [["classifica", "serie"], ["classifica_singola", "singola"]].forEach(function (t) {
      (g[t[0]] || []).forEach(function (v) {
        var chiave = t[1] + "|" + v.nome + "|" + v.punti + "|" + v.data;
        if (!inviati[chiave] && v.punti > 0) voci.push({ chiave: chiave, nome: v.nome, punti: v.punti, tipo: t[1] });
      });
    });
    if (!voci.length) return;
    var v = voci[0];
    fetch(window.OPENRUN_API, { method: "POST", credentials: "same-origin",
      headers: { "Content-Type": "application/json", "X-WP-Nonce": window.OPENRUN_NONCE || "" },
      body: JSON.stringify({ nome: v.nome, punti: v.punti, tipo: v.tipo }) })
      .then(function (r) {
        if (r.ok || r.status === 400) { // 400: dati scartati dal sito, non si riprova
          inviati[v.chiave] = 1;
          try { localStorage.setItem("openrun_inviati", JSON.stringify(inviati)); } catch (e) {}
        }
      }, function () {});
  }
  setInterval(inviaPunteggi, 35000);

  // Salvataggi (classifica, nome): si scrivono anche quando si chiude la pagina.
  function salva() { try { if (Module.FS) Module.FS.syncfs(false, function () {}); } catch (e) {} }
  window.addEventListener("pagehide", salva);
  document.addEventListener("visibilitychange", function () { if (document.hidden) salva(); });
})();
