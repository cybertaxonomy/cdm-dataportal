<?php
// $Id$

/**
 * @file
 * Adds helpful messages for website administrators
 * and provides documentation in /admin/help
 */

//html paths for differents documentation sections:
define('HELP_OVERVIEW', drupal_get_path('module', 'cdm_dataportal') . '/help/overview.html');
/*
define('HELP_SETTINGS_GENERAL', './sites/all/modules/cdm_dataportal/help/settings_general.html');
define('HELP_SETTINGS_GEO', './sites/all/modules/cdm_dataportal/help/settings_geo.html');
define('HELP_SETTINGS_LAYOUT', './sites/all/modules/cdm_dataportal/help/settings_layout.html');
define('HELP_SETTINGS_CACHE', drupal_get_path('module', 'cdm_dataportal') . '/help/settings_cache.html');
*/

function cdm_dataportal_menu_help ($may_cache, &$items) {

	if (!$may_cache) {

		$items[] = array(
	  'path' => 'admin/help/cdm_dataportal_general',
      'title' => t('general'),
      'callback' => 'cdm_dataportal_file_get_content',
      'callback arguments' => array(HELP_SETTINGS_GENERAL),
	  'access' => true,
      'type' => MENU_LOCAL_TASK,
		);
		
		$items[] = array(
	  'path' => 'admin/help/cdm_dataportal_geo',
      'title' => t('geo'),
      'callback' => 'cdm_dataportal_file_get_content',
      'callback arguments' => array(HELP_SETTINGS_GEO),
	  'access' => true,
      'type' => MENU_LOCAL_TASK,
		);
			
		$items[] = array(
	  'path' => 'admin/help/cdm_dataportal_layout',
      'title' => t('layout'),
      'callback' => 'cdm_dataportal_file_get_content',
      'callback arguments' => array(HELP_SETTINGS_LAYOUT),
	  'access' => true,
      'type' => MENU_LOCAL_TASK,
		);
		
		$items[] = array(
	  'path' => 'admin/help/cdm_dataportal_cache',
      'title' => t('cache'),
      'callback' => 'cdm_dataportal_file_get_content',
      'callback arguments' => array(HELP_SETTINGS_CACHE),
	  'access' => true,
      'type' => MENU_LOCAL_TASK,
		);
	}
}

/**
 * Implementation of hook_help().
 */
function cdm_dataportal_help($path) {
	//result to return
	$res = '';
	
	switch ($path) {

		case 'admin/help#cdm_dataportal':
			//$popup = (module_exists('advanced_help')) ? theme('advanced_help_topic', 'cdm_help', 'website-overview') : '';
			//var_dump($popup);
			$content = cdm_dataportal_file_get_content(HELP_OVERVIEW);
			$res = $content;
			break;

		default:
			$path_aux = str_replace('/' , '_' , $path);
			$res = theme('cdm_dataportal_' . $path_aux);
	}

	return $res;
}

function cdm_dataportal_file_get_content ($path) {
	
	//modyfing the html file to get the drupal paths 
	$content = file_get_contents($path);
	$content_result = str_replace("{MODULE_PATH}", file_directory_path(), $content);

	/*
	//converting from html to apt for maven documentation
	$apt_content = $content_result;
	//general modifications	
	$apt_content = str_replace('<p>', '', $apt_content);
	$apt_content = str_replace('</p>', '', $apt_content);
	$apt_content = str_replace('<b>', '<<', $apt_content);
	$apt_content = str_replace('</b>', '>>', $apt_content);
	$apt_content = str_replace('<i>', '<', $apt_content);
	$apt_content = str_replace('</i>', '>', $apt_content);
	$apt_content = str_replace('<h4>', '', $apt_content);
	$apt_content = str_replace('</h4>', '', $apt_content);
	$apt_content = str_replace('<h3>', '', $apt_content);
	$apt_content = str_replace('</h3>', '', $apt_content);
	$apt_content = str_replace('<ul>', '', $apt_content);
	$apt_content = str_replace('</ul>', '', $apt_content);
	$apt_content = str_replace('&amp', '&', $apt_content);
	$apt_content = str_replace('<li>', '*', $apt_content);
	$apt_content = str_replace('</li>', '', $apt_content);
    //special modifications
	$apt_content = str_replace('<a target="_blank" href="./?q=admin/build/modules">Administer&#45&#62Site buildin&#45&#62Modules</a>', 
	                           '<Administer-\>Site building-\>Modules>', 
	                           $apt_content);
    $apt_content = str_replace('<a target="_blank" href="./?q=admin/build/themes">Administer&#45&#62Site buildin&#45&#62Themes</a>',
	                           '<Administer-\>Site building-\>Themes>', 
	                           $apt_content);
	$apt_content = str_replace('<a target="_blank" href="./?q=admin/settings/cdm_dataportal">Administer&#45&#62Site Configuration&#45&#62CDM Dataportal</a>',
	                           '<Administer-\>Site Configuration-\>CDM Dataportal>', 
	                           $apt_content);
	$apt_content = str_replace('<a target="_blank" href="./?q=admin/settings/cdm_dataportal/general" title="General">here</a>',
	                           '<Administer-\>Site Configuration-\>CDM Dataportal>', 
	                           $apt_content);         
	$apt_content = str_replace('<a target="_blank" href="./?q=admin/settings/cdm_dataportal/geo" title="General">Geo &amp Map settings</a>',
	                           '<Administer-\>Site Configuration-\>CDM Dataportal-\>geo & Map>', 
	                           $apt_content);
	$apt_content = str_replace('<a target="_blank" href="./?q=admin/settings/cdm_dataportal/layout/search">Search Layout Settings</a>',
	                           '<Administer-\>Site Configuration-\>CDM Dataportal-\>Layout-\>Search>', 
	                           $apt_content);
	$apt_content = str_replace('<a target="_blank" href="./?q=admin/settings/cdm_dataportal/layout/media">Media Layout Settings</a>',
	                           '<Administer-\>Site Configuration-\>CDM Dataportal-\>Layout-\>Media>', 
	                           $apt_content);
//	                           
//    $apt_content = str_replace('<a target="_blank" href="./?q=admin/settings/cdm_dataportal/layout/media">Media Layout Settings</a>',
//	                           '<Administer-\>Site Configuration-\>CDM Dataportal-\>Layout-\>Media>', 
//	                           $apt_content);
//	 
		                           
	//regular expressions
	$apt_content = preg_replace('/<!--.*-->/', '', $apt_content);
	$apt_content = preg_replace('/\<a target="(.*)" href="(.*)" title="(.*)"\>(.*)\<\/a\>/', '{{{$2}$4}}', $apt_content);
	                                      
	$apt_file = str_replace('html', 'apt', $path);
	$fd = fopen($apt_file, 'w');
	fwrite($fd, $apt_content);
	fclose($fd);
	*/
	return $content_result;
}

function theme_cdm_dataportal_admin_settings_cdm_dataportal_general () {
	$res = t('<p>Help text for this page</p>');
	return $res;
}

function theme_cdm_dataportal_admin_settings_cdm_dataportal_geo () {
	$res = t('<p>Help text for this page</p>');	
	return $res;
}

function theme_cdm_dataportal_admin_settings_cdm_dataportal_layout () {
	$res = t('<p>Help text for this page</p>');
	return $res;
}

function theme_cdm_dataportal_admin_settings_cdm_dataportal_cache () {
	$res = t('<p>Help text for this page</p>');
	return $res;
}
