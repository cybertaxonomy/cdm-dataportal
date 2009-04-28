<?php

/**
 * Sets the body-tag class attribute.
 *
 * Adds 'sidebar-left', 'sidebar-right' or 'sidebars' classes as needed.
 */
function phptemplate_body_class($sidebar_left, $sidebar_right) {
	if ($sidebar_left != '' && $sidebar_right != '') {
		$class = 'sidebars';
	}
	else {
		if ($sidebar_left != '') {
			$class = 'sidebar-left';
		}
		if ($sidebar_right != '') {
			$class = 'sidebar-right';
		}
	}

	if (isset($class)) {
		print ' class="'. $class .'"';
	}
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
	if (!empty($breadcrumb)) {
		if($breadcrumb[0] == 'softwareTracker'){
			unset($breadcrumb[0]);
			return '<div class="breadcrumb">'. implode(' | ', $breadcrumb) .'</div>';
		} else {
			return '<div class="breadcrumb">'. implode(' › ', $breadcrumb) .'</div>';
		}
	}
}

/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $type = null) {
	static $node_type;
	if (isset($type)) $node_type = $type;

	if (!$content || $node_type == 'forum') {
		return '<div id="comments">'. $content . '</div>';
	}
	else {
		return '<div id="comments"><h2 class="comments">'. t('Comments') .'</h2>'. $content .'</div>';
	}
}

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function _phptemplate_variables($hook, $vars) {
	if ($hook == 'page') {

		if ($secondary = menu_secondary_local_tasks()) {
			$output = '<span class="clear"></span>';
			$output .= "<ul class=\"tabs secondary\">\n". $secondary ."</ul>\n";
			$vars['tabs2'] = $output;
		}

		// Hook into color.module
		if (module_exists('color')) {
			_color_page_alter($vars);
		}
		return $vars;
	}
	return array();
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
	$output = '';

	if ($primary = menu_primary_local_tasks()) {
		$output .= "<ul class=\"tabs primary\">\n". $primary ."</ul>\n";
	}

	return $output;
}

function phptemplate_render_edit_top_navigation(){
	$output = '';
	$output .= '<div class="edit-top-nav">';
	$output .= '<div class="nav-button"><a href="javascript:history.back();">' . t('« Back') . '</a></div>';
	$output .= '</div>';
	return $output;
}

/*
 * function to enhace views_ui.modules image button
 */
function phptemplate_views_imagebutton($element) {
  return '<input type="image" class="form-'. $element['#button_type'] .'" name="'. $element['#name'] .'" value="'. check_plain($element['#default_value']) .'" id="' . $element['#id'] . '" '. drupal_attributes($element['#attributes']) . ' src="' . $element['#image'] . '" alt="' . $element['#title'] . '" title="' . $element['#title'] . "\" />\n";
}