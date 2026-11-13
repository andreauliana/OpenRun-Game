<?php /* @package _tk */ get_header(); ?>
<?php /* Open Run al posto dell'immagine 404: il gioco sta nella cartella openrun/ del tema
         (si rigenera con motore_nuovo/web/build_web.sh, nel repo OpenRun-Game). */
$openrun = trailingslashit( get_stylesheet_directory_uri() ) . 'openrun/'; ?>
<div class="container">
	<div class="row">
		<div class="col s12">
			<h1 style="text-align: center;">Page Not Found</h1>
			<h4 style="text-align: center;">Sorry, but the page you requested has not been found!</h4>
		</div>
	</div>
</div>
<link rel="stylesheet" href="<?php echo esc_url( $openrun . 'openrun.css' ); ?>">
<style>#openrun-box { --or-spazio: 96px; } /* il menu a pillola del sito resta sopra il gioco */</style>
<!-- OPENRUN_BLOCCO -->
<script>
	window.OPENRUN_BASE = "<?php echo esc_js( $openrun ); ?>";
	window.OPENRUN_HOME = "<?php echo esc_js( home_url( '/' ) ); ?>";
</script>
<script src="<?php echo esc_url( $openrun . 'avvio.js' ); ?>"></script>
<br>
<?php get_footer(); ?>
