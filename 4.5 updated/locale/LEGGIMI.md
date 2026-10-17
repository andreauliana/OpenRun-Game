# Modello locale a minimo consumo (non ancora attivo negli agenti)
1. Installa Ollama (ollama.com). Poi: `sh prova_locale.sh prova` (scarica qwen3:4b, circa 2,5 GB, e fa un test).
2. Verifica la licenza del modello prima di usarlo: Qwen3 4B e' Apache-2.0 (controlla la pagina del modello su ollama.com).
3. Se il test risponde e la temperatura resta accettabile: aggiungi `opencode_snippet.json` a `sglavinated_src/opencode.json` (unendo la sezione "provider"), poi `opencode models` deve elencare `ollama/qwen3:4b`.
4. Solo dopo: aggiungi `opencode/ollama/qwen3:4b` in fondo a strumenti/panchina.txt (riserva, mai primo).
5. Quando non serve: `sh prova_locale.sh stop`.
Note: su questo Mac (Intel i7-4870HQ, 16 GB) il modello e' lento (qualche token al secondo). Usalo solo per lavori notturni semplici.
