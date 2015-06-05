<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * A QUICK OVERVIEW OF DRUPAL THEMING
 *
 *   The default HTML for all of Drupal's markup is specified by its modules.
 *   For example, the comment.module provides the default HTML markup and CSS
 *   styling that is wrapped around each comment. Fortunately, each piece of
 *   markup can optionally be overridden by the theme.
 *
 *   Drupal deals with each chunk of content using a "theme hook". The raw
 *   content is placed in PHP variables and passed through the theme hook, which
 *   can either be a template file (which you should already be familiary with)
 *   or a theme function. For example, the "comment" theme hook is implemented
 *   with a comment.tpl.php template file, but the "breadcrumb" theme hooks is
 *   implemented with a theme_breadcrumb() theme function. Regardless if the
 *   theme hook uses a template file or theme function, the template or function
 *   does the same kind of work; it takes the PHP variables passed to it and
 *   wraps the raw content with the desired HTML markup.
 *
 *   Most theme hooks are implemented with template files. Theme hooks that use
 *   theme functions do so for performance reasons - theme_field() is faster
 *   than a field.tpl.php - or for legacy reasons - theme_breadcrumb() has "been
 *   that way forever."
 *
 *   The variables used by theme functions or template files come from a handful
 *   of sources:
 *   - the contents of other theme hooks that have already been rendered into
 *     HTML. For example, the HTML from theme_breadcrumb() is put into the
 *     $breadcrumb variable of the page.tpl.php template file.
 *   - raw data provided directly by a module (often pulled from a database)
 *   - a "render element" provided directly by a module. A render element is a
 *     nested PHP array which contains both content and meta data with hints on
 *     how the content should be rendered. If a variable in a template file is a
 *     render element, it needs to be rendered with the render() function and
 *     then printed using:
 *       <?php print render($variable); ?>
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. With this file you can do three things:
 *   - Modify any theme hooks variables or add your own variables, using
 *     preprocess or process functions.
 *   - Override any theme function. That is, replace a module's default theme
 *     function with one you write.
 *   - Call hook_*_alter() functions which allow you to alter various parts of
 *     Drupal's internals, including the render elements in forms. The most
 *     useful of which include hook_form_alter(), hook_form_FORM_ID_alter(),
 *     and hook_page_alter(). See api.drupal.org for more information about
 *     _alter functions.
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   If a theme hook uses a theme function, Drupal will use the default theme
 *   function unless your theme overrides it. To override a theme function, you
 *   have to first find the theme function that generates the output. (The
 *   api.drupal.org website is a good place to find which file contains which
 *   function.) Then you can copy the original function in its entirety and
 *   paste it in this template.php file, changing the prefix from theme_ to
 *   STARTERKIT_. For example:
 *
 *     original, found in modules/field/field.module: theme_field()
 *     theme override, found in template.php: STARTERKIT_field()
 *
 *   where STARTERKIT is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_field() function.
 *
 *   Note that base themes can also override theme functions. And those
 *   overrides will be used by sub-themes unless the sub-theme chooses to
 *   override again.
 *
 *   Zen core only overrides one theme function. If you wish to override it, you
 *   should first look at how Zen core implements this function:
 *     theme_breadcrumbs()      in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called theme hook suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node--forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and theme hook suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440 and http://drupal.org/node/1089656
 */

/**
 * @param $which_image
 *   name of the image, see _zen_dataportal_imagenames() for possible values
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $default_image
 *  An file path relative to the theme folder
 * @param $css_selector
 *   The dom element to apply the image as background image
 * @param $background_style
 *   Addtional css setting for the background css attribute, default is  'scroll repeat'
 *
 * @see _zen_dataportal_imagenames() for possible values
 */
function _set_image_url($which_image, &$variables, $default_image = null, $css_selector = NULL, $background_style = 'scroll repeat') {


  if (!theme_get_setting('default_' . $which_image)) {
    $path = theme_get_setting($which_image . '_path');
    if(isset($path)){
      if (file_uri_scheme($path) == 'public') {
        $url = file_create_url($path);
      }
    }
  }

  if(!isset($url) && isset($default_image)) {
      $url = base_path() . path_to_theme() . '/' . $default_image;
  }

  if(isset($url)) {
    $variables[$which_image . '_url'] = $url;

    if($css_selector) {
      if(!isset($variables['inline_styles'])) {
        $variables['inline_styles'] = array();
      }
      $variables['inline_styles'][] = $css_selector . ' {' . "\n"
          . ' background: white url(\'' . check_url($url) .'\')  ' . $background_style  . ";\n"
          . '}';
    }
  }
}

function _color_gradient($color_1, $color_2) {

  $css = '';
  $css .= sprintf('background: %1$s; /* for non-css3 browsers */', $color_1) . "\n";
  $css .= sprintf('filter: progid:DXImageTransform.Microsoft.gradient(startColorstr=\'%1$s\', endColorstr=\'%2$s\'); /* for IE */', $color_1, $color_2) . "\n";
  $css .= sprintf('background: -webkit-gradient(linear, left top, left bottom, from(%1$s), to(%2$s)); /* for webkit browsers */', $color_1, $color_2) . "\n";
  $css .= sprintf('background: -moz-linear-gradient(top,  %1$s,  %2$s); /* for firefox 3.6+ */', $color_1, $color_2) . "\n";
  return $css;
}

/**
 *
 */
function _add_inline_styles(&$variables) {

  $css = array();
  if(!isset($variables['inline_styles'])) {
    $variables['inline_styles'] = array();
  }

  // site_name
  if(theme_get_setting('site-name_color')) {
      $variables['inline_styles'][] = '#site-name a span {color:' . check_plain(theme_get_setting('site-name_color')) . ';}';
  }
  // main-menu_color
  if(theme_get_setting('main-menu_color')) {
    $variables['inline_styles'][] = '#main-menu a:link, #main-menu a:visited, #main-menu a.active:link {color:' . check_plain(theme_get_setting('main-menu_color')) . ';} ';
    $variables['inline_styles'][] = '#navigation #main-menu ul.links li {border-color:' . check_plain(theme_get_setting('main-menu_color')) . ';}';
  }
  if(theme_get_setting('main-menu_background-color')) {
    if(theme_get_setting('main-menu_background-color-2')) {
      // with gradient
      $variables['inline_styles'][] = '#main-menu {'
        . _color_gradient(
              theme_get_setting('main-menu_background-color'),
              theme_get_setting('main-menu_background-color-2'))
        . '}';
    } else {
      $variables['inline_styles'][] = '#main-menu {background-color:' . check_plain(theme_get_setting('main-menu_background-color')) . ';}';
    }
  }
  if(theme_get_setting('sub-header_background-color')) {
    $variables['inline_styles'][] = '#sub-header {background-color:' . check_plain(theme_get_setting('sub-header_background-color')) . ';}';
  }
  if(theme_get_setting('header_margin_bottom') && theme_get_setting('site_name_position') != 'below_banner') {
    $variables['inline_styles'][] = '#sub-header {min-height: 0; height:' . check_plain(theme_get_setting('header_margin_bottom')) . ';}';
  }
  if(theme_get_setting('logo_size')) {
    $logo_size = theme_get_setting('logo_size');
    $zen_gutter_width = 40; // in px; must be same as $zen-gutter-width in zen_dataportal/sass/layouts/responsive-sidebars.scss
    $variables['inline_styles'][] = '#header {background-position:' . ($logo_size['width'] + $zen_gutter_width / 2) . 'px 0;}';
    $variables['inline_styles'][] = '
    /**
     * On small displays the main menu and sub-header must not have extra padding
     * on the left side.
     */
     @media all and (min-width: 480px) {
       #main-menu, #sub-header {padding-left:' . $logo_size['width'] . 'px;}
     }';
  }
  // as last styles the user_defined_styles which can be entered into a text field
  if(theme_get_setting('user_defined_styles')) {
    $variables['inline_styles'][] = check_plain(theme_get_setting('user_defined_styles'));
  }
}

/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("maintenance_page" in this case.)
 */
/* -- Delete this line if you want to use this function
function STARTERKIT_preprocess_maintenance_page(&$variables, $hook) {
  // When a variable is manipulated or added in preprocess_html or
  // preprocess_page, that same work is probably needed for the maintenance page
  // as well, so we can just re-use those functions to do that work here.
  STARTERKIT_preprocess_html($variables, $hook);
  STARTERKIT_preprocess_page($variables, $hook);
}
// */

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
function zen_dataportal_preprocess_html(&$variables, $hook) {
  _set_image_url('body_background', $variables, NULL, 'body');
  _set_image_url('page_background', $variables, NULL, '#page');
  _set_image_url('banner', $variables, 'banner.jpg', '#header', "scroll no-repeat; background-color: white; background-clip: content-box");
  _add_inline_styles($variables);

}

/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function zen_dataportal_preprocess_page(&$variables, $hook) {

  // site_name_position can be 'hidden', 'above_banner', or 'below_banner'
  $variables['site_name_position'] = 'above_banner';
  if(theme_get_setting('site_name_position')) {
    $variables['site_name_position'] = check_plain(theme_get_setting('site_name_position'));
  }
  // header_margin_bottom value can be any css length measurement
  $variables['header_margin_bottom'] = '0';
  if(theme_get_setting('header_margin_bottom')) {
    $variables['header_margin_bottom'] = check_plain(theme_get_setting('header_margin_bottom'));
  }

}

/**
 * Override or insert variables into the node templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function STARTERKIT_preprocess_node(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // STARTERKIT_preprocess_node_page() or STARTERKIT_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $variables['node']->type;
  if (function_exists($function)) {
    $function($variables, $hook);
  }
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function STARTERKIT_preprocess_comment(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the region templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
/* -- Delete this line if you want to use this function
function STARTERKIT_preprocess_region(&$variables, $hook) {
  // Don't use Zen's region--sidebar.tpl.php template for sidebars.
  //if (strpos($variables['region'], 'sidebar_') === 0) {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('region__sidebar'));
  //}
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function STARTERKIT_preprocess_block(&$variables, $hook) {
  // Add a count to all the blocks in the region.
  // $variables['classes_array'][] = 'count-' . $variables['block_id'];

  // By default, Zen will use the block--no-wrapper.tpl.php for the main
  // content. This optional bit of code undoes that:
  //if ($variables['block_html_id'] == 'block-system-main') {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('block__no_wrapper'));
  //}
}
// */
