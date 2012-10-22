<?php
/**
 * @file
 * Adds helpful messages for website administrators
 * and provides documentation in /admin/help.
 */

// Html paths for differents documentation sections:
define('HELP_OVERVIEW', drupal_get_path('module', 'cdm_dataportal') . '/help/overview.html');

// Comment @WA: @todo: fill these html files with help texts.
// But why to use different help texts here from the ones used in
// the settings forms?
define('HELP_SETTINGS_GENERAL', drupal_get_path('module', 'cdm_dataportal') . '/help/settings_general.html');
define('HELP_SETTINGS_GEO', drupal_get_path('module', 'cdm_dataportal') . '/help/settings_geo.html');
define('HELP_SETTINGS_LAYOUT', drupal_get_path('module', 'cdm_dataportal') . '/help/settings_layout.html');
define('HELP_SETTINGS_CACHE', drupal_get_path('module', 'cdm_dataportal') . '/help/settings_cache.html');

/**
 * Used in cdm_dataportal.module.
 */
function cdm_dataportal_menu_help(&$items) {

  $items['admin/help/cdm_dataportal_general'] = array(
    'title' => 'general',
    'page callback' => 'cdm_dataportal_file_get_content',
    'page arguments' => array(HELP_SETTINGS_GENERAL),
    'access callback' => 'user_is_logged_in',
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/help/cdm_dataportal_geo'] = array(
    'title' => 'geo',
    'page callback' => 'cdm_dataportal_file_get_content',
    'page arguments' => array(HELP_SETTINGS_GEO),
    'access callback' => 'user_is_logged_in',
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/help/cdm_dataportal_layout'] = array(
    'title' => 'layout',
    'page callback' => 'cdm_dataportal_file_get_content',
    'page arguments' => array(HELP_SETTINGS_LAYOUT),
    'access callback' => 'user_is_logged_in',
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/help/cdm_dataportal_cache'] = array(
    'title' => 'cache',
    'page callback' => 'cdm_dataportal_file_get_content',
    'page arguments' => array(HELP_SETTINGS_CACHE),
    'access callback' => 'user_is_logged_in',
    'type' => MENU_LOCAL_TASK,
  );
}

/**
 * Implements hook_help().
 */
function cdm_dataportal_help($path, $arg) {
  // Result to return.
  $res = '';

  switch ($path) {
    case 'admin/help#cdm_dataportal':
      // $popup = (module_exists('advanced_help')) ? theme('advanced_help_topic', 'cdm_help', 'website-overview') : '';
      // var_dump($popup);
      $content = cdm_dataportal_file_get_content(HELP_OVERVIEW);
      $res = $content;
      break;

    default:
      $path_aux = str_replace('/', '_', $path);
      if (function_exists('theme_cdm_portal_' . $path_aux)) {
        $res = theme('cdm_dataportal_' . $path_aux, array());
      }
  }

  return $res;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cdm_dataportal_file_get_content($path) {

  // Modifing the html file to get the drupal paths.
  $content = file_get_contents($path);
  $content_result = str_replace("{MODULE_PATH}", file_build_uri($path), $content);

  /* XXXXX change made because of problems with SVN!!! XXXXX
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
   //                             '<Administer-\>Site Configuration-\>CDM Dataportal-\>Layout-\>Media>',
   //                             $apt_content);
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

/**
 * See hook_theme in cdm_dataportal.module.
 */
function theme_cdm_dataportal_admin_config_cdm_dataportal_general() {
  $res = t('<p>Help text for this page</p>');
  // @WA this could also link to admin/help/cdm_dataportal_cache ?
  // Instead of theme_more_help_link we use a custom one here, to mimic
  // a D5 style link.
  $res .= theme('cdm_dataportal_admin_config_more_help_link', array(
    'url' =>'admin/help/cdm_dataportal')
  );
  return $res;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_cdm_dataportal_admin_config_cdm_dataportal_geo() {
  $res = t('<p>Help text for this page</p>');
  $res .= theme('cdm_dataportal_admin_config_more_help_link', array(
    'url' =>'admin/help/cdm_dataportal')
  );
  return $res;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_cdm_dataportal_admin_config_cdm_dataportal_layout() {
  $res = t('<p>Help text for this page</p>');
  $res .= theme('cdm_dataportal_admin_config_more_help_link', array(
    'url' =>'admin/help/cdm_dataportal')
  );
  return $res;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_cdm_dataportal_admin_config_cdm_dataportal_cachesite() {
  $res = t('<p>Help text for the cache site settings page</p>');
  $res .= theme('cdm_dataportal_admin_config_more_help_link', array(
    'url' =>'admin/help/cdm_dataportal')
  );
  return $res;
}

/**
 * Custom theme to use in admin config pages.
 *
 * Use instead of theme_more_help_link to have D5 style more-help links
 * instead of D7 style.
 *
 * @author w.addink <w.addink@eti.uva.nl>
 */
function theme_cdm_dataportal_admin_config_more_help_link($variables) {
  $html = '<div class="more-help-link cdm-dataportal-settings-more-help-link">[';
  $html .= l(t('more help...'), $variables['url']);
  $html .= ']</div>';
  return $html;
}
