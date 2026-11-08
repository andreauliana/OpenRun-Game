Turno 1 08/11/2026 09:47 — @developer
Blocco: RIV — tuning bump rivale + fix flip
Passo: modificato bumpRallentaFattore_ da 0.85 a 0.80 (-15% a -20% velocità dopo contatto rivale, range Andrea 08/11); disabilito colpiMemoria_ per rivali (ribaltamento solo per fuori strada prolungato); rebuilt e gate verificatione
File cambiati: motore_nuovo/src/game/game.cpp (line 2493)
Comando: sh UFFICIO/Officina/TOOL/gate_t1.sh soak
Exit code: 0