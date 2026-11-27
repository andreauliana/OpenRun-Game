# RIASSUNTO — @capo (27/11/2026, T2 ora 2/12 verifica)
- **Tappa:** MIG1 [~] + V47 [~] continuo (∞). CP-001/005/006/007 CHIUSI; CP-002/003/004 APERTI. ANDREA_REVIEW prec.ass. §7 nessuna RISPOSTA; legacy 08/11/vocabolario/REGOLA BLOCCATA §2 intoccati.
- **D20 bloccante:** 5 hunk `drawPlayerCar` (renderer.cpp 2798-2816,2875-2889,2890-2905,2910-2922,2932-2952); serve autorizzazione Andrea o revert. Nessuna chiusura CP su D20.
- **Handoff:** dev 27/11 09:20 V9 lv06 g44 no mod, percorso exit 0 (2125f,35.4s,0%), smoke 0.671/4.006ms replay ID3627137594→lv01 g45; jr 27/11 09:21 VJ5 src invariato, rilievo paesaggio.cpp:260-262 disponiTerraPonte O(n) (no REGOLA BLOCCATA), VJ3 lv05 FAIL 2.40% VJ4 6/6; gf 27/11 09:24 g257 0 conv exit 0 (3 tool+autotest 43/42/37).
- **Checkpoint:** CHK auto SCAMBIO_RECENTE non peer; gate non rieseguito dal capo (rif 25/11 13:04 exit 0); 0 CP chiudibili.
- **Deleghe:** @developer D20 revert/autorizzazione poi V9 lv01 g45+indaga VJ3 (rev @jr); @jr verifica D20/VJ3 lv05 log+VJ5∞ (rev @dev/@capo); @grafico VG3∞ (rev @capo cat/lic/prov).
- **Blocchi/DEFAULT:** D20 attende decisione Andrea (DEFAULT revert, no merge; capo non scrive ANDREA_REVIEW.md); VJ3 lv05 FAIL aperto; Pi 400 non verificato; furgone `.png.png`/pose `_b` non toccare; nessun FINITO (∞).
