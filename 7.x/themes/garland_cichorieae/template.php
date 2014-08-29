<?php
/**
 * @file
 * Overrides of generic themeing functions in cdm_dataportal.theme.php.
 */

/*
  ***** GARLAND OVERRIDES *****
*/

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
    print ' class="' . $class . '"';
  }
}


/**
 * Override or insert PHPTemplate variables into the templates.
 */
function _phptemplate_variables($hook, $vars) {
  if ($hook == 'page') {

    if ($secondary = menu_secondary_local_tasks()) {
      $output = '<span class="clear"></span>';
      $output .= "<ul class=\"tabs secondary\">\n" . $secondary . "</ul>\n";
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
 * Returns HTML for the rendered local tasks.
 *
 * The default implementation renders them as tabs.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
  $output = '';

  if ($primary = menu_primary_local_tasks()) {
    $output .= "<ul class=\"tabs primary\">\n" . $primary . "</ul>\n";
  }

  return $output;
}

/**
 * Returns HTML for taxon list thumbnails.
 *
 * Theme specific mods:
 * $captionElements = array('title', '#uri'=>t('open Image'));
 * $mediaLinkType:
 * "NORMAL": link to the image page or to the $alternativeMediaUri if it is
 * defined instead of
 * "LIGHTBOX": open the link in a light box,
 * TODO expose those in admin section, by adding 'em to gallery_settings see
 * http://dev.e-taxonomy.eu/trac/ticket/2494.
 *
 * @param array $variables
 *   An associative array containing:
 *   - taxon: The taxon object for which to theme the thumbnails.
 *
 * @ingroup themeable
 */
function garland_cichorieae_cdm_taxon_list_thumbnails($variables) {
  $out = '';
  $taxon = $variables['taxon'];

  $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SEARCH_GALLERY_NAME);

  $mediaLinkType = 'NORMAL';
  $showCaption = $gallery_settings['cdm_dataportal_show_thumbnail_captions'];
  $captionElements = array();
  if ($showCaption) {
    $captionElements = array('title', '#uri' => t('open Image'));
  }

  $gallery_name = $taxon->uuid;

  $galleryLinkUri = path_to_taxon($taxon->uuid, 'images');

  $mediaList = $mediaList = _load_media_for_taxon($taxon);

  $out .= theme('cdm_media_gallerie', array(
    'mediaList' => $mediaList,
    'galleryName' => $gallery_name,
    'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
    'cols' => $gallery_settings['cdm_dataportal_media_cols'],
    'maxRows' => $gallery_settings['cdm_dataportal_media_maxRows'],
    'captionElements' => $captionElements,
    'mediaLinkType' => $mediaLinkType,
    'alternativeMediaUri' => $galleryLinkUri,
    'showCaption' => NULL,
  ));

  return $out;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function garland_cichorieae_cdm_feature_name($variables) {
  $feature_name = $variables['feature_name'];
  switch ($feature_name) {
    case "Protologue":
      return t("Original Publication");
    default:
      return ucfirst($feature_name);
  }
}

/*
======== Special functions for subtheme handling=============
*/

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function sub_theme() {
  global $user, $custom_theme;

  // Only select the user selected theme if it is available in the
  // list of enabled themes.
  $themes = list_themes();
  $theme = $user->theme && $themes[$user->theme]->status ? $user->theme : variable_get('theme_default', 'garland');

  // Allow modules to override the present theme... only select custom theme
  // if it is available in the list of installed themes.
  $theme = $custom_theme && $themes[$custom_theme] ? $custom_theme : $theme;

  return $theme;
}

/**
 * Return the path to the currently selected sub theme.
 */
function path_to_sub_theme() {
  $themes = list_themes();
  $theme = sub_theme();
  return dirname($themes[$theme]->filename);
}

/**
 * Implements hook_preprocess_HOOK() for theme_page().
 *
 * Assign the css classes primary-links and secondary-links to the menus.
 * Modify primary-links if cdm_api module exists.
 */
function garland_cichorieae_preprocess_page(&$vars) {
  if (isset($vars['main_menu'])) {
    /*
    obsolete, see http://dev.e-taxonomy.eu/trac/ticket/2191
    if(module_exists('cdm_api')){ foreach($vars['main_menu'] as $key =>
    $menu_item){ // nb this makes each menu item you click opening a new
    browser window..
    $menu_item['attributes']['target']=generalizeString($menu_item['title']);
    $main_menu[] = $menu_item; }; }else{ $main_menu = $vars['main_menu'] ; };
    */
    $vars['primary_nav'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'], 'attributes' => array(
        'class' => array(
          'links', 'inline', 'main-menu', 'primary-links',
        )),
        'heading' => array(
          'text' => t('Main menu'), 'level' => 'h2', 'class' => array(
            'element-invisible',
          ))));
  }
  else {
    $vars['primary_nav'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_nav'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'], 'attributes' => array(
        'class' => array(
          'links',
          'inline',
          'secondary-menu',
          'secondary-links',
        )),
        'heading' => array(
          'text' => t('Secondary menu'),
          'level' => 'h2',
          'class' => array('element-invisible'),
        )));
  }
  else {
    $vars['secondary_nav'] = FALSE;
  }

  /*
  Display node title as page title for the comment form.
  Comment @WA: it would probably be better to select $uuid from node_cdm
  table and link to cdm_dataportal/taxon/%uuid instead.
  */
  if (arg(0) == 'comment' && arg(1) == 'reply') {
    $node = $vars['page']['content']['system_main']['comment_node']['#node'];
    $vars['title'] = l(check_plain($node->title), 'node/' . $node->nid);
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_node().
 *
 * Fixes file urls in nodes. In nodes, relative urls are used to include files
 * like <img src="/files/..
 *
 * Portals can be installed in configurations with
 * sub-directories however, in which case these urls need to be adjusted.
 * Examples: mysite.org, mysite.org/myportal, mysite.org/portals/myportal.
 *
 * Therefore preprocess nodes and replace these urls with a the appropriate url
 * for the current setup.
 *
 * @author W.Addink <w.addink@eti.uva.nl>
 */
function garland_cichorieae_preprocess_node(&$vars) {
  $body = '';
  // Warning: use #markup value, for which filters like php, html etc are applied!
  if (isset($vars['content']['body'][0]['#markup'])) {
    $body = $vars['content']['body'][0]['#markup'];
  }
  else {
    $vars['fixed_body'] = '';
    return;
  }

  $file_path = '/' . variable_get('file_public_path', conf_path() . '/files');
  global $base_url;
  if ($base_url == '/') {
    drupal_set_message(t('
      The $base_url in this portal could not be set, please set the $base_url
      manually your Drupal settings.php file.', 'error'
    ));
  }
  $fixed_file_path = $base_url . $file_path;
  $preg_file_path = preg_quote($file_path, '/');
  $body = preg_replace('/src\s*=\s*["]\s*' . $preg_file_path . '/', 'src="' . $fixed_file_path, $body);
  $body = preg_replace('/src\s*=\s*[\']\s*' . $preg_file_path . '/', 'src=\'' . $fixed_file_path, $body);
  $body = preg_replace('/href\s*=\s*["]\s*' . $preg_file_path . '/', 'href="' . $fixed_file_path, $body);
  $body = preg_replace('/href\s*=\s*[\']\s*' . $preg_file_path . '/', 'href=\'' . $fixed_file_path, $body);

  $vars['fixed_body'] = $body;
}

/**
 * Implements hook_form_FORM_ID_alter() for comment_form().
 *
 * Alter the comment form to make it look like a D5 style comment form.
 *
 * @author W.Addink <w.addink@eti.uva.nl>
 */
function garland_cichorieae_form_comment_form_alter(&$form, &$form_state) {

  if (!isset($form['comment_preview'])) {
    $form['header'] = array(
      '#markup' => '<h2>' . t('Reply') . '</h2>',
      '#weight' => -2,
    );
  }
  $form['subject']['#title'] = $form['subject']['#title'] . ':';
  $form['comment_body']['und'][0]['#title'] = $form['comment_body']['und'][0]['#title'] . ':';
  if (isset($form['author']['_author']['#title'])) {
    $form['author']['_author']['#title'] = $form['author']['_author']['#title'] . ':';
  }
  $form['actions']['submit']['#value'] = t('Post comment');
  $form['actions']['submit']['#weight'] = 1000;
  $form['actions']['preview']['#value'] = t('Preview comment');
}

/**
 * Implements hook_preprocess_HOOK() for theme_comment().
 *
 * Alter the comment display to make it look like a D5 style comment.
 *
 * @author W.Addink <w.addink@eti.uva.nl>
 */
function garland_cichorieae_preprocess_comment(&$variables) {
  $comment = $variables['elements']['#comment'];
  if (isset($comment->subject)) {
    // Print title without link.
    $variables['title'] = $comment->subject;
    if ($variables['status'] == 'comment-preview') {
      // Add 'new' to preview.
      $variables['new'] = t('new');
    }
  }
}
