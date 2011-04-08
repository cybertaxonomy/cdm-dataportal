<?php
// $Id: template.php,v 1.1 2009/08/25 19:15:43 troy Exp $


/**
 * Return the different routes names from the pageroute database
 * tables without names repetition
 *
 * @return
 *   array of strings with the route names
 */
function _get_route_names(){
	$route_names = array();
	$result = db_query("SELECT r.path FROM {pageroute_routes} r");
	while ($data = db_fetch_object($result)) {
	   if (!in_array($data->path, $route_names)){
	   $route_names[] = $data->path;
	   }
	}
	return $route_names;
}

function _get_current_route_name(){
	return arg(0) . '/' . arg(1);
}

function _get_current_route_page(){
  return arg(2);
}

/**
 * Return a string with the new title
 *
 * @return
 *   string with the new name
 */
function _replace_route_title($old_title){
	$old_titles_list = array('cdm-setups/group' => 'CDM Setup for Groups',
	                         'cdm-setups/linux_community' => 'CDM Setup for Communities',
	                         'cdm-setups/windows_community' => 'CDM Setup for Communities');

	return $old_titles_list[$old_title];
}

function _replace_link_title($old_title){
	$new_title = $old_title;
	$result = db_query("SELECT  r.path, p.name, p.title FROM {pageroute_routes} r, {pageroute_pages} p WHERE r.prid=p.prid");

	//initialization of route data
	while ($data = db_fetch_object($result)) {
		$route_names[] = $data->path;
		$route_pages[] = $data->name;
		$route_titles[] = $data->title;
	}

	//replacing the route_name for route_title on the tabs
	if(in_array(_get_current_route_name() , $route_names)){
		$key = array_search($old_title, $route_pages);
	    if($key != NULL || $key !== FALSE){
	    	$new_title = $route_titles[$key];
	    }
	}

	return $new_title;
}

function phptemplate_preprocess_page(&$vars) {
  $defaults = array(
    'admin_left_column' => 1,
    'admin_right_column' => 0
  );

  global $theme_key;
  // Get default theme settings.
  $settings = theme_get_settings($theme_key);
  $settings = array_merge($defaults, $settings);

  if (arg(0) == 'admin' && ($settings['admin_right_column'] == 0) && !(arg(1) == 'build' && arg(2) == 'block')) {
    $vars['right'] = '';
  }

  if (arg(0) == 'admin' && ($settings['admin_left_column'] == 0) && !(arg(1) == 'build' && arg(2) == 'block')) {
    $vars['left'] = '';
  }

  $vars['registration_enabled'] = variable_get('user_register', 1);
  //$vars['closure'] .= '<span class="developer">
  //<strong><a href="http://russianwebstudio.com" title="Go to RussianWebStudio.com">Drupal theme</a></strong> by        <a href="http://russianwebstudio.com" title="Go to RussianWebStudio.com">RussianWebStudio.com</a> <span class="version">ver.1</span>
  //</span>';
}


/**
 * Generate the HTML representing a given menu item ID.
 *
 * An implementation of theme_menu_item_link()
 *
 * @param $link
 *   array The menu item to render.
 * @return
 *   string The rendered menu item.
 */
function phptemplate_menu_item_link($link) {
  if (empty($link['options'])) {
    $link['options'] = array();
  }

  if(strpos($link['title'], _get_current_route_page()) !== false){
    $link['options']['attributes']['class'] = 'active_node';
  }

  // If an item is a LOCAL TASK, render it as a tab
  if (module_exists("pageroute") && ($link['type'] & MENU_IS_LOCAL_TASK)) {
  	$new_title = _replace_link_title($link['title']);
    //$link['title'] = '<span class="tab">' . check_plain($link['title']) . '</span>';
  } else {
     $new_title = $link['title'];
  }
    $link['title'] = '<span class="tab">' . $new_title . '</span>';
    $link['options']['html'] = TRUE;

  if (empty($link['type'])) {
    $true = TRUE;
  }

  $attributes = array();

  return l($link['title'], $link['href'], $link['options']);
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clear-block to tabs.
 */
function phptemplate_menu_local_tasks() {
  $output = '';
  if (module_exists("pageroute")){
    $route_names = _get_route_names();
  }


  if (isset($route_names) && ($primary = menu_primary_local_tasks()) && ( in_array(_get_current_route_name() , $route_names))) {
  	// it is a page root local task!
  	$li_count = substr_count($primary, "<li");

  	$li_width = floor(100 / $li_count).'%';
  	$primary = str_replace('<li', '<li style="width: '.$li_width.'"', $primary);

    $pos = strpos($primary, '<li');
  	$primary = substr_replace($primary, ' class="first" ', $pos + 3, 1);
  	$pos = strrpos($primary, '<li');
    $primary = substr_replace($primary, ' class="last" ', $pos + 3, 1);

    //$a = str_replace('_',' ',_get_current_route_name());
    //replacing the navigation banner title
    $new_title = _replace_route_title(_get_current_route_name());
    $title = '<h2 class="pageroute_title">' .  $new_title . '</h2>';
  	$output .= '<div class="pageroute pageroute_'.str_replace('/', '_', strtolower(_get_current_route_name())).'">'.$title.'<ul class="tabs primary clear-block">'. $primary .'</ul>'.'</div>';

  } elseif ($primary = menu_primary_local_tasks()) {
    $output .= '<ul class="tabs primary clear-block">' . $primary . '</ul>';
  }

  if ($secondary = menu_secondary_local_tasks()) {
    $output .= '<ul class="tabs secondary clear-block">' . $secondary . '</ul>';
  }

  return $output;
}

function summertime_cdm_node_submitted($node) {
  return t('by !username on @datetime',
    array(
      '!username' => theme('username', $node),
      '@datetime' => format_date($node->created),
    ));
}

