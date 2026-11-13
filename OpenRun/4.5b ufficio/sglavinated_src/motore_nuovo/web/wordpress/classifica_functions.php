/* ===== Open Run: classifica online (incollare in fondo a functions.php del tema) ===== */
/* Salva i punteggi in un'impostazione di WordPress (opzione "openrun_classifica"):
   niente tabelle da creare. GET  /wp-json/openrun/v1/classifica  -> le due classifiche
                           POST /wp-json/openrun/v1/classifica  -> {nome, punti, tipo} */
add_action( 'rest_api_init', function () {
	register_rest_route( 'openrun/v1', '/classifica', array(
		array( 'methods' => 'GET', 'permission_callback' => '__return_true',
			'callback' => function () { return openrun_classifica_leggi(); } ),
		array( 'methods' => 'POST', 'permission_callback' => '__return_true',
			'callback' => 'openrun_classifica_scrivi' ),
	) );
} );

function openrun_classifica_leggi() {
	$c = get_option( 'openrun_classifica', array() );
	return array( 'serie' => isset( $c['serie'] ) ? $c['serie'] : array(),
	              'singola' => isset( $c['singola'] ) ? $c['singola'] : array() );
}

function openrun_classifica_scrivi( WP_REST_Request $r ) {
	// 1) arriva dalla pagina del gioco (token della pagina)
	if ( ! wp_verify_nonce( $r->get_header( 'x_wp_nonce' ), 'wp_rest' ) )
		return new WP_Error( 'openrun', 'token', array( 'status' => 403 ) );
	// 2) al massimo un punteggio ogni 30 secondi per indirizzo
	$ip = md5( isset( $_SERVER['REMOTE_ADDR'] ) ? $_SERVER['REMOTE_ADDR'] : '' );
	if ( get_transient( 'openrun_ip_' . $ip ) )
		return new WP_Error( 'openrun', 'troppo presto', array( 'status' => 429 ) );
	// 3) dati plausibili: nome come nel gioco (6 caratteri A-Z 0-9 .), punti nel massimo possibile
	$nome  = strtoupper( trim( (string) $r->get_param( 'nome' ) ) );
	$punti = (int) $r->get_param( 'punti' );
	$tipo  = $r->get_param( 'tipo' ) === 'singola' ? 'singola' : 'serie';
	if ( ! preg_match( '/^[A-Z0-9. ]{1,6}$/', $nome ) || $punti < 1 || $punti > 3000 )
		return new WP_Error( 'openrun', 'dati', array( 'status' => 400 ) );
	set_transient( 'openrun_ip_' . $ip, 1, 30 );
	// 4) si tiene la Top 20 di ogni classifica
	$c = openrun_classifica_leggi();
	$c[ $tipo ][] = array( 'nome' => $nome, 'punti' => $punti, 'data' => gmdate( 'Y-m-d' ) );
	usort( $c[ $tipo ], function ( $a, $b ) { return $b['punti'] - $a['punti']; } );
	$c[ $tipo ] = array_slice( $c[ $tipo ], 0, 20 );
	update_option( 'openrun_classifica', $c, false );
	return $c;
}
/* ===== fine Open Run ===== */
