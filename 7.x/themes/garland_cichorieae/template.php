<?php
/**
 * @file
 * Overrides of generic themeing functions in cdm_dataportal.theme.php.
 */

/**
 * Returns HTML for a cdm_taxon_page_profile.
 *
 * The description page is supposed to be the front page for a taxon.
 *
 * @param array $variables
 *   An associative array containing:
 *   - taxon: The taxon object displayed on the taxon page.
 *   - mergedTrees
 *   - media:
 *   - hideImages: boolean, FALSE if images should be hided.
 *
 * @ingroup themeable
 */
function garland_cichorieae_cdm_taxon_page_profile($variables) {
  $taxon = $variables['taxon'];
  $mergedTrees = $variables['mergedTrees'];
  $media = $variables['media'];
  $hideImages = $variables['hideImages'];

  $out = '';
  global $base_url;

  RenderHints::pushToRenderStack('taxon_page_description');
  // Description TOC.
  $out = theme('cdm_featureTreeTOCs', array('mergedTrees' => $mergedTrees));

  // Preferred image.
  // 2 lines hard coded for testing.
  if (variable_get('cdm_dataportal_show_default_image', FALSE) && !$hideImages) {

    // $defaultPreferredImage = drupal_get_path('theme',
    // 'garland_cichorieae').'/images/nopic_400x300.jpg';
    $defaultRepresentationPart = FALSE;
    $defaultRepresentationPart->width = 400;
    $defaultRepresentationPart->height = 300;
    $defaultRepresentationPart->uri = $base_url . '/' . drupal_get_path('theme', 'garland_cichorieae') . '/images/nopic_400x300_4x3cm.jpg';

    $imageUriParams = '&width=400&height=300&quality=95&format=jpeg';

    $imageMaxExtend = 400;
    $out .= '<div id="taxonProfileImage">' . theme('cdm_preferredImage', array(
      'media' => $media,
      'defaultRepresentationPart' => $defaultRepresentationPart,
      'imageMaxExtend' => $imageMaxExtend,
      'parameters' => $imageUriParams,
      )) . '</div>';
  }

  // Description.
  $out .= theme('cdm_featureTrees', array(
    'mergedTrees' => $mergedTrees,
    'taxon' => $taxon,
  ));
  RenderHints::popFromRenderStack();

  return $out;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function garland_cichorieae_cdm_descriptionElementTextData($variables) {
  $element = $variables['element'];
  $asListElement = $variables['asListElement'];
  $feature_uuid = $variables['feature_uuid'];

  $description = '';
  if (isset($element->multilanguageText_L10n->text)) {
    $description = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
  }
  $sourceRefs = '';
  $result = array();
  $res_author;
  $res_date;
  $do_links = TRUE;
  $default_theme = variable_get('theme_default', 'garland_cichorieae');

  if (($default_theme == 'flora_malesiana' || $default_theme == 'flore_afrique_centrale' || $default_theme == 'flore_gabon') && $element->feature->titleCache == 'Citation') {
    $asListElement = TRUE;
  }
  elseif ($element->feature->uuid == UUID_CHROMOSOMES_NUMBERS) {
    $asListElement = TRUE;
  }
  else {
    $asListElement = FALSE;
  }

  // Printing annotations footnotes.
  $annotation_fkeys = theme('cdm_annotations_as_footnotekeys', array(
    'cdmBase_list' => $element,
    'footnote_list_key' => $feature_uuid,
  ));

  if ($feature_uuid == UUID_NAME_USAGE || $feature_uuid == UUID_CHROMOSOMES) {
    $do_links = FALSE;
  }

  if (is_array($element->sources)) {
    foreach ($element->sources as $source) {
      // Initialize some variables.
      if ($feature_uuid == UUID_NAME_USAGE) {
        $referenceCitation = cdm_ws_get(
          CDM_WS_NOMENCLATURAL_REFERENCE_CITATION,
          array($source->citation->uuid),
          "microReference=" . urlencode($source->citationMicroReference
        ));
        $referenceCitation = $referenceCitation->String;
      }
      else {
        $referenceCitation = theme('cdm_DescriptionElementSource', array(
          'descriptionElementSource' => $source,
          'doLink' => $do_links,
        ));
      }
      if ($description && strlen($description) > 0 && $referenceCitation) {
        $sourceRefs .= ' (' . $referenceCitation . ')';
      }
      elseif ($referenceCitation) {
        $sourceRefs = $referenceCitation;
      }
      // Generate the output.
      if (strlen($sourceRefs) > 0) {
        $sourceRefs = '<span class="sources">' . $sourceRefs . '</span>';
      }
      $name_used_in_source_link_to_show = '';
      if (isset($source->nameUsedInSource->uuid) && ($feature_uuid != UUID_NAME_USAGE)) {
        // Do a link to name page.
        $name_used_in_source_link_to_show = l($source->nameUsedInSource->titleCache, path_to_name($source->nameUsedInSource->uuid), array(), NULL, NULL, FALSE, TRUE);
      }
      elseif (isset($source->nameUsedInSource->uuid) && ($feature_uuid == UUID_NAME_USAGE)) {
        // Do not do link for NAME USAGE feature.
        $name_used_in_source_link_to_show = $source->nameUsedInSource->titleCache;
      }
      elseif (isset($source->nameUsedInSource->originalNameString) && strlen($source->nameUsedInSource->originalNameString) > 0) {
        // Show a text without link.
        $name_used_in_source_link_to_show = $source->nameUsedInSource->originalNameString;
      }

      if ($asListElement && ($feature_uuid == UUID_NAME_USAGE)) {
        $out = '<li class="descriptionText">' . $name_used_in_source_link_to_show;
        // Adding ":" if necesary.
        if (!empty($name_used_in_source_link_to_show) && (!empty($description) || !empty($sourceRefs))) {
          $out .= ': ';
        }

        $out .= $description . $sourceRefs . theme('cdm_annotations_as_footnotekeys', array(
          'cdmBase_list' => $element,
          'footnote_list_key' => $feature_uuid,
        )) . $annotation_fkeys . '</li>';
      }
      elseif ($asListElement) {

        $out = '<li class="descriptionText DescriptionElement">';
        // Adding ":" if necessary.
        if (!empty($name_used_in_source_link_to_show)) {
          if ( (!empty($description)|| !empty($sourceRefs)) && $feature_uuid != UUID_CHROMOSOMES_NUMBERS) {
            $out .= $name_used_in_source_link_to_show . ': ';
          } else {
            $out .= $name_used_in_source_link_to_show . ' ';
          }
        }

        $out .= $description . $sourceRefs . theme('cdm_annotations_as_footnotekeys', array(
          'cdmBase_list' => $element,
          'footnote_list_key' => $feature_uuid,
        )) . $annotation_fkeys . '</li>';
        // Special handling for flora malesiana.
        // TODO: possible better way to implement this case?
      }
      else {
        if (isset($name_used_in_source_link_to_show)) {
          $name_used_in_source_link_to_show = ' (name in source: ' . $name_used_in_source_link_to_show . ')';
        }
        if (!isset($description)) {
          $description = '';
        }
        if (!isset($sourceRefs)) {
            $sourceRefs = '';
        }
        if (!isset($name_used_in_source_link_to_show)) {
            $name_used_in_source_link_to_show = '';
        }
        if (!isset($annotation_fkeys)) {
            $annotation_fkeys = '';
        }
        $out = '<span class="' . html_class_atttibute_ref($element) . '"> ' . $description . $sourceRefs . $name_used_in_source_link_to_show . $annotation_fkeys . '</span>';
      }
    }
  }

  // If no sources, print the description.
  if (!isset($out)) {
    $out = '<span class="' . html_class_atttibute_ref($element) . '"> ' . $description . $annotation_fkeys . '</span>';
  }

  /* Dead code:
   *
   * Disabled code; can this be removed?
  if ($feature_uuid == UUID_NAME_USAGE){ foreach($element->sources as
  $source){ $referenceCitation =
  cdm_ws_get(CDM_WS_NOMENCLATURAL_REFERENCE_CITATION,
  array($source->citation->uuid),
  "microReference=".urlencode($source->citationMicroReference));
  $referenceCitation = $referenceCitation->String; if($description &&
  strlen($description) > 0 && $referenceCitation ){ $sourceRefs .= '
  ('.$referenceCitation.')' ; }else if ($referenceCitation){ $sourceRefs =
  $referenceCitation; } } }else{ foreach($element->sources as $source){
  $referenceCitation = theme('cdm_DescriptionElementSource', $source,
  ($feature_uuid == UUID_NAME_USAGE) ? FALSE : TRUE); if($description &&
  strlen($description) > 0 && $referenceCitation ){ $sourceRefs .= '
  ('.$referenceCitation.')' ; }else if ($referenceCitation){ $sourceRefs =
  $referenceCitation; } } } if(strlen($sourceRefs) > 0){ $sourceRefs = '<span
  class="sources">' . $sourceRefs . '</span>'; } if
  ($source->nameUsedInSource->uuid && ($feature_uuid != UUID_NAME_USAGE)){
  //do a link to name page $name_used_in_source_link_to_show =
  l($source->nameUsedInSource->titleCache,
  path_to_name($source->nameUsedInSource->uuid), array(), NULL, NULL, FALSE
  ,TRUE); }else if ($source->nameUsedInSource->uuid && ($feature_uuid ==
  UUID_NAME_USAGE)){ //do not do link for NAME USAGE feature
  $name_used_in_source_link_to_show = $source->nameUsedInSource->titleCache;
  }else if (strlen($source->nameUsedInSource->originalNameString) > 0){
  //show a text without link $name_used_in_source_link_to_show =
  $source->nameUsedInSource->originalNameString; } if($asListElement &&
  ($feature_uuid == UUID_NAME_USAGE)){ $out = '<li class="descriptionText">'
  . $name_used_in_source_link_to_show; //adding ":" if necesary if
  ($name_used_in_source_link_to_show && ($description || $sourceRefs)){ $out
  .= ': '; } $out .= $description . $sourceRefs .
  theme('cdm_annotations_as_footnotekeys', $element) . '</li>'; }else if
  ($asListElement){ $out = '<li class="descriptionText">' .
  $name_used_in_source_link_to_show; //adding ":" if necesary if
  ($name_used_in_source_link_to_show && ($description || $sourceRefs)){ $out
  .= ': '; } $out .= $description . $sourceRefs .
  theme('cdm_annotations_as_footnotekeys', $element) . '</li>'; //special
  handling for flora malesiana TODO: possible better way to implement this
  case? }else{ if ($name_used_in_source_link_to_show){
  $name_used_in_source_link_to_show = ' (name in source: '.
  $name_used_in_source_link_to_show . ')'; } $out = $description .
  $sourceRefs . $name_used_in_source_link_to_show; $out .=
  theme('cdm_annotations_as_footnotekeys', $element); }
  */
  // Add annotations as footnote key.
  // $out .= theme('cdm_annotations_as_footnotekeys', $element); move above.
  return $out;
}

// NO LONGER NEEDED - since term representation is changed now in the cdm for
// the cichorieae
// ====> delete
// /**
// * @overrides theme_cdm_taggedtext2html in order to replace t.infr and
// t.infgen. with '[unranked]'
// */
// function garland_cichorieae_cdm_taggedtext2html(array &$taggedtxt, $tag =
// 'span', $glue = ' ', $skiptags = array()){
// $out = '';
// $i = 0;
// foreach($taggedtxt as $tt){
// if(!in_array($tt->type, $skiptags) && strlen($tt->text) > 0){
// $out .= (strlen($out) > 0 && ++$i < count($taggedtxt)? $glue : '').'<'.$tag.'
// class="'.$tt->type.'">';
// if($tt->type == "rank" && ($tt->text == "t.infr." || $tt->text ==
// "t.infgen.")){
// $out .= t('[unranked]');
// }else{
// $out .= t($tt->text);
// }
// $out .= '</'.$tag.'>';
// }
// }
// return $out;
// }
/**
 * Returns HTML for a cdm_descriptionElementArray.
 *
 * @param array $variables
 *   An associative array containing:
 *   - elementArray: The array with CDM description elements being formatted.
 *   - feature: The description feature container object for the
 *     description element array.
 *   - glue: String for joining the description elements together.
 *   - sortArray: Boolean, TRUE if the array should be sorted before themeing.
 *   - enclosingHTML: HTML tag, e.g. 'div', in which to wrap the description.
 *
 * @ingroup themeable
 */
function garland_cichorieae_cdm_descriptionElementArray($variables) {
  $elementArray = $variables['elementArray'];
  $feature = $variables['feature'];
  $glue = $variables['glue'];
  $sortArray = $variables['sortArray'];
  $enclosingHtml = $variables['enclosingHtml'];

  $enclosingHtml = 'div';
  $out = '<' . $enclosingHtml . ' class="description" id="' . $feature->representation_L10n . '">';

  if ($sortArray) {
    sort($elementArray);
  }

  $out .= join($elementArray, $glue);

  $out .= '</' . $enclosingHtml . '>';
  return $out;
}

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
 * Allow themable wrapping of all comments.
 */
/*
function phptemplate_comment_wrapper($content, $type = NULL) {
  static $node_type;
  if (isset($type)) $node_type = $type;

  if (! $content || $node_type == 'forum') {
    return '<div id="comments">' . $content . '</div>';
  }
  else {
    return '<div id="comments"><h2 class="comments">' . t('Comments') . '</h2>' . $content . '</div>';
  }
}
*/

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
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function garland_cichorieae_get_partDefinition($variables) {
  if ($variables['nameType'] == 'BotanicalName') {
    return array(
      'namePart' => array('name' => TRUE),
      'nameAuthorPart' => array('name' => TRUE, 'authors' => TRUE),
      'referencePart' => array('reference' => TRUE, 'microreference' => TRUE),
      'statusPart' => array('status' => TRUE),
      'descriptionPart' => array('description' => TRUE),
    );
  }
  return FALSE;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function garland_cichorieae_get_nameRenderTemplate($variables) {
  $template = array();

  switch ($variables['renderPath']) {
    case 'taxon_page_title':
    case 'polytomousKey':
      $template = array(
        'namePart' => array('#uri' => TRUE),
      );
      break;
    case 'taxon_page_synonymy':
    case 'related_taxon':
      $template = array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
        'statusPart' => TRUE,
        'descriptionPart' => TRUE,
      );
      break;
    case 'acceptedFor':
      $template = array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
      );
      break;
    case 'typedesignations':
    case 'list_of_taxa':
    case '#DEFAULT':
      $template = array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
      );
  }
  return $template;
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

  $mediaQueryParameters = array("type" => "ImageFile");
  $galleryLinkUri = path_to_taxon($taxon->uuid, 'images');

  // TODO cdm_dataportal_show_media = ????
  $selectShowMedia = variable_get('cdm_dataportal_show_media', 0);
  if ($selectShowMedia == 0) {
    $mediaList = cdm_ws_get(
      CDM_WS_PORTAL_TAXON_MEDIA,
      array($taxon->uuid),
      queryString($mediaQueryParameters
    ));
  }
  else {
    $mediaList = cdm_ws_get(
      CDM_WS_PORTAL_TAXON_SUBTREE_MEDIA,
      array($taxon->uuid),
      queryString($mediaQueryParameters
    ));
  }
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
