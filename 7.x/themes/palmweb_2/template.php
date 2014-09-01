<?php
/**
 * @file
 * Overrides of generic themeing functions in cdm_dataportal.theme.php.
 */

/**
 * This operride theme function ignores the original sources and annotations
 * and renders a hardcoded citation string at the end of the list of distibution
 * elements.
 * TODO the same output could also be achieved by collection all source citations
 * as it is done by the footnodesystem and to print the list of citations at the end
 * of the area list. The footnote key must be omitted in this case of course.
 *
 */

function palmweb_2_cdm_descriptionElement_Distribution($variables) {
  $descriptionElements = $variables['descriptionElements'];
  $enclosingTag = $variables['enclosingTag'];

  $out = '';
  $separator = ', ';

  RenderHints::pushToRenderStack('descriptionElementDistribution');
  RenderHints::setFootnoteListKey(UUID_DISTRIBUTION);

  $itemCnt = 0;
  foreach ($descriptionElements as $descriptionElement) {

    $out .= '<' . $enclosingTag . ' class="DescriptionElement DescriptionElement-' . $descriptionElement->class . '">';
    $out .= $descriptionElement->area->representation_L10n;
    if (++$itemCnt < count($descriptionElements)) {
      $out .= $separator;
    }
    $out .= "</" . $enclosingTag . ">";
  }
  $taxonTrees = cdm_ws_fetch_all(CDM_WS_PORTAL_TAXONOMY);
  $reference = new stdClass();
  foreach ($taxonTrees as $taxonTree) {
    if ($taxonTree->uuid == variable_get('cdm_taxonomictree_uuid')) {
      if (isset($taxonTree->reference)) {
        $reference = $taxonTree->reference;
      }
      break;
    }
  }
  $referenceCitation = '';
  if (isset($reference->uuid)) {
    $referenceCitation .= '(<span class="reference">';
    $referenceCitation .= l(t('World Checklist of Monocotyledons'), path_to_reference($reference->uuid), array('attributes' => array('class' => array('reference'))));
    $referenceCitation .= '</span>)';
  }
  else {
    // Comment @WA Added for compatibility with D5, but I think it is better to
    // remove this to not show a link rather than the wrong one.
    $referenceCitation .= '(<span class="reference">';
    $referenceCitation .= l(t('World Checklist of Monocotyledons'), '', array('attributes' => array('class' => array('reference'))));
    $referenceCitation .= '</span>)';
  }

  $sourceRefs = '';
  if ($out && strlen($out) > 0) {
    $sourceRefs = ' ' . $referenceCitation;
  }

  if (strlen($sourceRefs) > 0) {
    $sourceRefs = '<span class="sources">' . $sourceRefs . '</span>';
  }

  RenderHints::popFromRenderStack();
  return $out . $sourceRefs;

}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function palmweb_2_cdm_feature_nodes($variables){
  $mergedFeatureNodes = $variables['mergedFeatureNodes'];
  $taxon = $variables['taxon'];

  $out = '';
  RenderHints::pushToRenderStack('feature_nodes');

  $gallery_settings = getGallerySettings(CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME);
  // Creating an array to place the description elements in.
  $bibliographyOut = array();
  $countFeatures = 0;
  $numberOfChildren = count(cdm_ws_get(CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON, array (get_taxonomictree_uuid_selected(), $taxon->uuid)));
  if ($taxon->name->rank->titleCache == "Genus") {
    $subRank = "species";
  }
  if ($taxon->name->rank->titleCache == "Species") {
    if($numberOfChildren==1){
      $subRank = "infraspecific taxon";
    }
    else{
      $subRank = "infraspecific taxa";
    }
  }
  if ($numberOfChildren != 0) {
    $out .= '<a name="number_of_taxa"> </a><H2>Number of Taxa</H2><div class="content"> <ul class="description">';
    $out .= '<li class=\"descriptionText DescriptionElement\">' . $numberOfChildren . " " . $subRank . '</li></ul>';
  }

  foreach ($mergedFeatureNodes as $node) {



    if (isset($node->descriptionElements['#type']) || has_feature_node_description_elements($node)) {

      $featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';
      $block = new stdclass(); // Empty object.
      $block->module = 'cdm_dataportal';
      // If the option is enabled the description elements will be added
      // to the array.
      $show_bibliography = variable_get('cdm_show_bibliography', 1);
      if ($show_bibliography) {
        $bibliographyOut[] = $node->descriptionElements;
      }
      $media_list = array();
      if ($node->feature->uuid != UUID_IMAGE && $node->feature->uuid != UUID_USE) {
        $countFeatures++;
        $countFeatures++;
        $block->delta = generalizeString($featureRepresentation);
        $block->subject = '<span class="' . html_class_attribute_ref($node->feature) . '">'
            . theme('cdm_feature_name',  array('feature_name' => $featureRepresentation)) . '</span>';
        $block->module = "cdm_dataportal-feature";
        $block->content = '';

      /*
         * Content/DISTRIBUTION.
         */
        if ($node->feature->uuid == UUID_DISTRIBUTION) {

          $distributionElements = null;
          $distribution_info_dto = null;
          $text_data_out_array = array();

          $distribution_sortOutArray = FALSE;
          if (variable_get('distribution_sort', 'NO_SORT') != 'NO_SORT') {
            $distribution_glue = '';
            $distribution_enclosingTag = 'dl';
          }
          else {
            $distribution_glue = '';
            $distribution_enclosingTag = 'ul';
          }

          if(!isset($node->descriptionElements['#type']) || !$node->descriptionElements['#type']=='DTO') {
            // skip the DISTRIBUTION section if there is no DTO type element
            continue;
          }

          if(isset($node->descriptionElements['TextData'])){
            // --- TextData

            $text_data_glue = '';
            $text_data_sort = FALSE;
            $text_data_enclosingTag = 'ul';

            foreach ($node->descriptionElements['TextData'] as $text_data_element){
              $asListElement = FALSE;
              $repr = theme('cdm_descriptionElementTextData', array(
                  'element' => $text_data_element,
                  'asListElement' => $asListElement,
                  'feature_uuid' => $text_data_element->feature->uuid,
              ));

              if (!array_search($repr, $text_data_out_array)) {
                $text_data_out_array[] = $repr;
                // TODO HINT: sorting in theme_cdm_feature_block_elements will
                // not work since this array contains html attributes with uuids
                // !!!!
                $text_data_sort = TRUE;
                $text_data_glue = '<br/> ';
                $text_data_enclosingTag = 'p';
              }
            }
          }


          if ($text_data_out_array && variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0)) {
            $tmp_render_array = compose_cdm_feature_block_elements(
              $text_data_out_array,
              $node->feature,
              $text_data_glue,
              $text_data_sort,
              $text_data_enclosingTag
            );

            $block->content .= $tmp_render_array['#markup'];
          }

          // --- Distribution map
          $distribution_map_query_parameters = null;
          if(isset($node->descriptionElements['DistributionInfoDTO'])) {
            $distribution_map_query_parameters = $node->descriptionElements['DistributionInfoDTO']->mapUriParams;
          }
          $map_render_element = compose_distribution_map($taxon, $distribution_map_query_parameters);
          $block->content .= $map_render_element['#markup'];

            // --- tree or list
              $dto_out_array = array();
          if(isset($node->descriptionElements['DistributionInfoDTO'])) {
            $distribution_info_dto = $node->descriptionElements['DistributionInfoDTO'];

            // --- tree
            if (is_object($distribution_info_dto->tree)) {
              $dto_out_array[] = theme('cdm_description_ordered_distributions', array('distribution_tree' => $distribution_info_dto->tree));
            }

            // --- sorted element list
            if( is_array($distribution_info_dto->elements) && count($distribution_info_dto->elements) > 0 ) {
              foreach ($distribution_info_dto->elements as $descriptionElement){
                if (is_object($descriptionElement->area)) {
                  $sortKey = $descriptionElement->area->representation_L10n;
                  $distributionElements[$sortKey] = $descriptionElement;
                }
              }
              ksort($distributionElements);
              $dto_out_array[] = theme('cdm_descriptionElement_Distribution', array(
                  'descriptionElements' => $distributionElements,
              ));

            }
            //

            $tmp_render_array = compose_cdm_feature_block_elements(
                $dto_out_array,
                $node->feature,
                $distribution_glue,
                $distribution_sortOutArray,
                $distribution_enclosingTag
            );
            $block->content .= $tmp_render_array['#markup'];
          }

          // --- TextData at the bottom
          if ($text_data_out_array && !variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0)) {
            $tmp_render_array = compose_cdm_feature_block_elements(
                $text_data_out_array,
                $node->feature,
                $text_data_glue,
                $text_data_sort,
                $text_data_enclosingTag
            );
            $block->content .= $tmp_render_array['#markup'];
          }

        }

        /*
        Content/COMMON_NAME.
        */
        elseif ($node->feature->uuid == UUID_COMMON_NAME) {
          $block->content .= theme('cdm_common_names', array('elements' => $node->descriptionElements));
        }

        /*
        Content/ALL OTHER FEATURES.
        */
        elseif ($node->feature->uuid == UUID_USE_RECORD) {
          $block->content .= theme('cdm_block_Uses', array('taxonUuid' => $taxon->uuid));
          // $block->content .= theme('cdm_descriptionElements', $node->descriptionElements, $node->feature->uuid, $taxon->uuid),
        }
        else {
          $block->content .= compose_cdm_descriptionElements($node->descriptionElements, $node->feature->uuid, $taxon->uuid);

          /*
          Content/ALL OTHER FEATURES/Subordinate Features
          subordinate features are printed inline in one floating text,
          it is expected that subordinate features only "contain" TextData
          elements.
          */
          // TODO move into own theme.
          if (isset($node->childNodes[0])) {

            // TODO support more than one level of children.
            // @see http://dev.e-taxonomy.eu/trac/ticket/2393/
            $text = '';
            foreach ($node->childNodes as $child) {

              if (isset($child->descriptionElements) && is_array($child->descriptionElements)) {
               foreach ($child->descriptionElements as $element) {

                 if (is_array($element->media)) {
                    // Append media of supordinate elements to list of main
                    // feature.
                   $media_list = array_merge($media_list, $element->media);
                 }

                 $description = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
                // TODO use localized version of feature name, the locale must
                // match the locale of the multilanguage text
                // (http://dev.e-taxonomy.eu/trac/ticket/2394).
                 $description = str_replace($element->feature->titleCache, '<em>' . $element->feature->titleCache . '</em>', $description);
               }
               $text .= " " . $description;
               $description = '';
             }
            }
            $block->content .= $text;
          }
        }

        /*
        Media/ALL FEATURES.
        */
        $media_list = array_merge($media_list, cdm_dataportal_media_from_descriptionElements($node->descriptionElements));

        $gallery = theme('cdm_media_gallerie', array(
           'mediaList' => $media_list,
           'galleryName' => CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME . '_' . $node->feature->uuid,
           'maxExtend' => isset($gallery_settings['cdm_dataportal_media_maxextend']) ? $gallery_settings['cdm_dataportal_media_maxextend'] : NULL ,
           'cols' => isset($gallery_settings['cdm_dataportal_media_cols']) ? $gallery_settings['cdm_dataportal_media_cols'] : NULL ,
           'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? $gallery_settings['cdm_dataportal_media_maxRows'] : NULL ,
           'captionElements' => isset($captionElements) ? $captionElements : NULL ,
           )
        );

        $block->content .= $gallery;
        $block->content .= theme('cdm_footnotes', array('footnoteListKey' => $node->feature->uuid));
        $block->content .= theme('cdm_annotation_footnotes', array('footnoteListKey' => $node->feature->uuid));

        // Add anchor to subject.
        $block->subject = '<a name="' . $block->delta . '"></a>' . $block->subject;

        $block->region = FALSE;
        $out .= theme('block', array('elements' => array(
          '#block' => $block,
          '#children' => $block->content,
        )));
      }
    }
  }

  // Add the display of the number of taxa in the selected genus.
  //


  // Calling the theme function for Bibliography to add it to the output.
  $show_bibliography = variable_get('cdm_show_bibliography', 1);
  if ($show_bibliography && $countFeatures != 0) {
    $out .= theme('cdm_descriptionElementBibliography', array('descriptionElementsBibliography' => $bibliographyOut));
  }

  RenderHints::popFromRenderStack();
  return $out;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function palmweb_2_cdm_search_results($variables){
  $pager = $variables['pager'];
  $path = $variables['path'];
  $out = '';

  $showThumbnails = isset($_SESSION['pageoption']['searchtaxa']['showThumbnails']) ? $_SESSION['pageoption']['searchtaxa']['showThumbnails'] : 0;
  if (!is_numeric($showThumbnails)) {
    // AT RBG KEW - 14/11/2011 - Set the show thumbnails to 0 by default.
    $showThumbnails = 0;
  }
  $setSessionUri = url('cdm_api/setvalue/session', array('query' => array('var' => '[pageoption][searchtaxa][showThumbnails]', 'val' => '')));
  drupal_add_js('jQuery(document).ready(function() {

        // Init.
        if(' . $showThumbnails . ' == 1){
              jQuery(\'.media_gallery\').show(20);
        } else {
          jQuery(\'.media_gallery\').hide(20);
        }
        // Add change hander.
        jQuery(\'#showThumbnails\').change(
          function(event){
            var state = 0;
            if(jQuery(this).is(\':checked\')){
              jQuery(\'.media_gallery\').show(20);
              state = 1;
            } else {
              jQuery(\'.media_gallery\').hide(20);
            }
            // Store state in session variable.
            var uri = \'' . $setSessionUri . '\' + state;
            jQuery.get(uri);
          });
        });', "inline");

  drupal_set_title(t('Search results'));

  // AT RBG KEW - 14/11/2011 - Changed the wording of the Show Thumbnails
  // tickbox text.
  $out .= '<div class="page_options">';
  $out .= '<form name="pageoptions">';
  $out .= '<input id="showThumbnails" type="checkbox" name="showThumbnails" ';
  $out .= $showThumbnails == 1 ? 'checked="checked"' : '';
  $out .= '> ' . t('Show Image Thumbnails') . '</form></div>';
  if (!empty($pager) && count($pager->records) > 0) {
      $out .= '<div id="search_results">';
    $out .= theme('cdm_list_of_taxa', array('records' => $pager->records));
    $out .= '</div>';
    $out .= theme('cdm_pager', array(
      'pager' => $pager,
      'path' => $path,
      'parameters' => $_REQUEST,
    ));
  }
  else {
    $out = '<h4 class="error">Sorry, no matching entries found.</h4>';
  }
  return $out;
}

/*
Comment @WA: theme function moved to cdm_dataportal module,
theme/cdm_dataportal.bibliography.theme so this can be used by other portals
as well.
@TODO: should this not be part of the palmweb_2 featuretree and be treated
as a normal description feature?
function theme_cdm_descriptionElementBibliography
function formatReference_for_Bibliography($references) {
 */

/**
 * @todo Please document this function.
 */
function palmweb_2_cdm_media_caption($variables){
  $media = $variables['media'];
  $elements = $variables['elements'];

  $media_metadata = cdm_read_media_metadata($media);

  $doTitle = !$elements || array_search('title', $elements) !== FALSE;
  $doDescription = !$elements || array_search('description', $elements) !== FALSE;
  $doArtist = !$elements || array_search('artist', $elements) !== FALSE;
  $doLocation = !$elements || array_search('location', $elements) !== FALSE;
  $doRights = !$elements || array_search('rights', $elements) !== FALSE;

  $descriptionPrefix = "";

  $out = '<dl class="media-caption">';
  // Title.
  if ($doTitle) {
    if ($media_metadata['title']) {
      $out .= '<dt class = "title">' . t('Title') . '</dt> <dd class = "title">' . $media_metadata['title'] . '</dd>';
      $descriptionPrefix = "- ";
    }
    elseif (!($doDescription && $media_metadata['description'])) {
      // Use filename as fallbackoption if no description will be shown.
      $out .= '<dt class = "title">' . t('Title') . '</dt> <dd class = "title">' . $media_metadata['filename'] . '</dd>';
      $descriptionPrefix = "- ";
    }
  }
  // Description.
  if ($media_metadata['description'] && $doDescription) {
    $out .= '<dt class = "description">' . t('Description') . '</dt> <dd class = "description">' . $descriptionPrefix . $media_metadata['description'] . '</dd>';
  }
  // Artist.
  if ($media_metadata['artist'] && $doArtist) {
    $out .= '<dt class = "artist">' . t('Artist') . '</dt> <dd class = "astist">' . str_replace("'","", $media_metadata['artist']) . '</dd>';
  }
  // Location.
  if ($doLocation) {
    $location = '';
    $location .= $media_metadata['location']['sublocation'];
    if ($location && $media_metadata['location']['city']) {
      $location .= ', ';
    }
    $location .= $media_metadata['location']['city'];
    if ($location && $media_metadata['location']['province']) {
      $location .= ', ';
    }
    $location .= $media_metadata['location']['province'];
    if ($location && $media_metadata['location']['country']) {
      $location .= ' (' . $media_metadata['location']['country'] . ')';
    }
    else {
      $location .= $media_metadata['location']['country'];
    }
    if ($location) {
      $out .= '<dt class = "location">' . t('Location') . '</dt> <dd class = "location">' . $location  . '</dd>';
    }
  }
  // Rights.
  if ($doRights) {
    $rights = '';
    // Copyrights.
    $cnt = count($media_metadata['rights']['copyright']['agentNames']);
    if ($cnt > 0) {
      $rights .= '<dt class="rights">&copy;</dt> <dd class="rights"> ';
      for ($i = 0; $i < $cnt; $i++) {
        $rights .= str_replace("'","", $media_metadata['rights']['copyright']['agentNames'][$i]);
        if ($i + 1 < $cnt) {
          $rights .= ' / ';
        }
      }
      $rights .= '</dd>';
    }
    // License.
    $cnt = count($media_metadata['rights']['license']['agentNames']);
    if ($cnt > 0) {
      $rights .= '<dt class ="license">' . t('License') . '</dt> <dd class = "license">';
      for ($i = 0; $i < $cnt; $i++) {
        $rights .= $media_metadata['rights']['license']['agentNames'][$i];
        if ($i + 1 < $cnt) {
          $rights .= ' / ';
        }
      }
      $rights .= '</dd>';
    }
    if ($rights) {
      $out .= $rights . '</dt>';
    }
  }
  // TODO add all other metadata elemenst generically.
  $out .= '</dl>';
  // Return value.
  return $out;
}

/**
 * Overrive of the original theme_cdm_reference()
 * the main difference here seems to be that
 * this function is completely omitting the citation title cache
 * and only sets the authorTeam as the
 * _short_form_of_author_team() as $citation.
 *
 * If the authorteam is not set citation was empty,
 * this has been fixed for http://dev.e-taxonomy.eu/trac/ticket/4261
 *
 * TODO can this be made configuable via the dataportal
 *      settings so that we can remove this function?
 */
function xxx_palmweb_2_cdm_reference($variables) {
  $reference = $variables['reference'];
  $microReference = $variables['microReference'];
  $doLink = $variables['doLink'];
  $referenceStyle = $variables['referenceStyle'];

  if(!isset($reference->authorTeam)){
    $author_team = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $reference->uuid);
  } else {
    $author_team = $reference->authorTeam;
  }

  $year = '';
  if (isset($reference->datePublished->start)) {
    $year = partialToYear($reference->datePublished->start);
  }
  if(isset($author_team->titleCache)){
    $citation = _short_form_of_author_team ($author_team->titleCache) . (!empty($year) ? '. ' . $year : '');
    $citation = str_replace('..', '.', $citation);
  } else {
    $citation = $reference->titleCache;
  }

  if ($doLink) {
    $out = '<span class="reference">';
    $out .= l($citation, path_to_reference($reference->uuid), array(
    'attributes' => array('class' => 'reference'),
    'absolute' => TRUE,
    'html' => TRUE,
    ));
    $out .= '</span>';
  }
  else {
    $out = '<span class="reference">' . $citation . '</span>';
  }
  // FIXME use microreference webservice instead.
  if (!empty($descriptionElementSource->citationMicroReference)) {
    $out .= ': ' . $descriptionElementSource->citationMicroReference;
  }

  return $out;
}

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
 * Allow themeable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $type = NULL) {
  static $node_type;
  if (isset($type)) {
    $node_type = $type;
  }

  if (!$content || $node_type == 'forum') {
    return '<div id="comments">' . $content . '</div>';
  }
  else {
    return '<div id="comments"><h2 class="comments">' . t('Comments') . '</h2>' . $content . '</div>';
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
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs.
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
function palmweb_2_cdm_feature_name($variables){
  $feature_name = $variables['feature_name'];
  switch ($feature_name) {
    case "Protologue": return t("Original Publication");
    default: return t(ucfirst($feature_name));
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_page().
 *
 * Assign the css classes primary-links and secondary-links to the menus and
 * process the 'Login' menu item, to change into 'My account' after login and
 * change the tab title for the IMCE file browser.
 *
 * @author W.Addink <w.addink@eti.uva.nl>
 */
function palmweb_2_preprocess_page(&$vars) {

  if (isset($vars['main_menu'])) {
    // For the Palmae theme we want to change the menu item 'Login' into
    // 'My account' if a user is logged in.
    global $user;
    foreach ($vars['main_menu'] as $key => $value) {
        if ($value['href'] == 'user' && !empty($user->name)) {
            $vars['main_menu'][$key]['title'] = t('My account');
            $vars['main_menu'][$key]['href'] = 'user/' . $user->uid;
        }
    }
    // Theme the main menu with the desired css classes.
    $vars['primary_nav'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('links', 'inline', 'main-menu', 'primary-links'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )));
  }
  else {
    $vars['primary_nav'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_nav'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'inline', 'secondary-menu', 'secondary-links'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )));
  }
  else {
    $vars['secondary_nav'] = FALSE;
  }

  // Change IMCE tab to 'Personal Files'.
  if (!empty($vars['tabs']['#primary'])) {
    foreach ($vars['tabs']['#primary'] as $key => $value) {
      if ($value['#link']['path'] == 'user/%/imce') {
        $vars['tabs']['#primary'][$key]['#link']['title'] = t('Personal Files');
      }
    }
  }


  /* Display node title as page title for the comment form.
  * Comment @WA: it would probably be better to select $uuid from node_cdm
  * table and link to cdm_dataportal/taxon/%uuid instead.
  */
  if (arg(0) == 'comment' && arg(1) == 'reply') {
      $node = $vars['page']['content']['system_main']['comment_node']['#node'];
      $vars['title'] = l(check_plain($node->title),'node/' . $node->nid);
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
function palmweb_2_preprocess_node(&$vars) {
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
  $body = preg_replace ('/src\s*=\s*["]\s*' . $preg_file_path . '/', 'src="' . $fixed_file_path , $body);
  $body = preg_replace ('/src\s*=\s*[\']\s*' . $preg_file_path . '/', 'src=\'' . $fixed_file_path , $body);
  $body = preg_replace ('/href\s*=\s*["]\s*' . $preg_file_path . '/', 'href="' . $fixed_file_path , $body);
  $body = preg_replace ('/href\s*=\s*[\']\s*' . $preg_file_path . '/', 'href=\'' . $fixed_file_path , $body);

  $vars['fixed_body'] = $body;
}

/**
 * Implements hook_form_FORM_ID_alter() for comment_form().
 *
 * Alter the comment form to make it look like a D5 style comment form.
 *
 * @author W.Addink <w.addink@eti.uva.nl>
 */
function palmweb_2_form_comment_form_alter(&$form, &$form_state) {

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
function palmweb_2_preprocess_comment(&$variables) {
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
