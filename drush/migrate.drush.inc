<?php



function migrate_drush_help($section) {
	switch ($section) {
		case 'drush:migrate-refresh':
			return dt("Refresh the drupal site by reloading all modules [ module_list(TRUE) ]"
			." and then all themes [ list_themes(TRUE); ] ."
			." This will update the database with the server specific path to the themes and modules."
			." Use this command after migrating to a different server via sql dump,"
			." before opening any page.");
	}
}

function migrate_drush_command() {
	$items = array();

	$items['migrate-refresh'] = array(
            'description' => "Refresh the drupal site by reloading all modules [ module_list(TRUE) ]"
            ." and then all themes [ list_themes(TRUE); ] ."
            ." This will update the database with the server specific path to the themes and modules."
            ." Use this command after migrating to a different server via sql dump,"
            ." before opening any page.",
            'aliases' => array('mire'),
            'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
            );

     return $items;
}

function drush_migrate_refresh() {
	drush_print("migrate-refresh");
	$modules = module_list(TRUE);
	drush_print( count($modules) . " modules loaded");
	$themes = list_themes(TRUE);
	drush_print( count($themes) . " available themes");
}