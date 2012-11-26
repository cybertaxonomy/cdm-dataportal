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
function palmweb_2_cdm_taxon_page_profile($variables){

  $taxon = $variables['taxon'];
  $mergedTrees = $variables['mergedTrees'];
  $media = $variables['media'];
  $hideImages = $variables['hideImages'];

  $out = '';

  if (!$hideImages) {
    // Preferred image.
    // Hardcoded for testing.
    $defaultRepresentationPart = new stdClass();
    $defaultRepresentationPart->width = 184;
    $defaultRepresentationPart->height = 144;
    $defaultRepresentationPart->uri = drupal_get_path('theme', 'palmweb_2') . '/images/no_picture.png';

    // Preferred image size 184px × 144.
    $imageMaxExtend = 184;
    $out .= '<div id="taxonProfileImage">';
    $out .= theme('cdm_preferredImage', array(
      'media' => $media,
      'defaultRepresentationPart' => $defaultRepresentationPart,
      'imageMaxExtend' => $imageMaxExtend,
    ));
    $out .= '</div>';
  }

  // Description TOC.
  $out .= theme('cdm_featureTreeTOCs', array('mergedTrees' => $mergedTrees));

  // Description.
  $out .= theme('cdm_featureTrees', array('mergedTrees' => $mergedTrees, 'taxon' => $taxon));

  return $out;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function palmweb_2_cdm_descriptionElementDistribution($variables) {
  $descriptionElements = $variables['descriptionElements'];
  $enclosingTag = $variables['enclosingTag'];

  $out = '';
  $separator = ', ';

  RenderHints::pushToRenderStack('descriptionElementDistribution');
  RenderHints::setFootnoteListKey(UUID_DISTRIBUTION);

  $itemCnt = 0;
  foreach ($descriptionElements as $descriptionElement) {
    /*
    $out .= ($descriptionElement->class);
    // Annotations as footnotes.
    $annotationFootnoteKeys = theme('cdm_annotations_as_footnotekeys', $descriptionElement);
    // Source references as footnotes.
    $sourcesFootnoteKeyList = '';
    foreach($descriptionElement->sources as $source){
      $_fkey = FootnoteManager::addNewFootnote(UUID_DISTRIBUTION, theme('cdm_DescriptionElementSource', $source, FALSE));
      $sourcesFootnoteKeyList .= theme('cdm_footnote_key', $_fkey, ($sourcesFootnoteKeyList ? $separator : ''));
    }
    if($annotationFootnoteKeys && $sourcesFootnoteKeyList){
      $annotationFootnoteKeys .= $separator;
    }
    */
        $out .= '<' . $enclosingTag . ' class="DescriptionElement DescriptionElement-' . $descriptionElement->class . '">';
        // $out .= $descriptionElement->area->representation_L10n . $annotationFootnoteKeys . $sourcesFootnoteKeyList;
        $out .= $descriptionElement->area->representation_L10n;
        if (++$itemCnt < count($descriptionElements)) {
          $out .= $separator;
        }
        $out .= "</" . $enclosingTag . ">";
  }
  $taxonTrees = cdm_ws_get(CDM_WS_PORTAL_TAXONOMY);
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
function palmweb_2_cdm_feature_nodesTOC($variables){
  $featureNodes = $variables['featureNodes'];
  $out = '';

  global $theme;

  $out .= '<ul>';
  $countFeatures = 0;
  $numberOfChildren = count(cdm_ws_get(CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON, array (
    get_taxonomictree_uuid_selected(),
    substr(strrchr($_GET["q"],'/'), 1),
  )));
  if ($numberOfChildren != 0) {
    $out .= '<li>';
    $out .= l(t(theme('cdm_feature_name', array('feature_name' => 'Number of Taxa'))), $_GET['q'], array(
      'attributes' => array('class' => array('toc')),
      'fragment' => generalizeString('Number Of Taxa'),
    ));
    $out .= '</li>';
  }
  foreach ($featureNodes as $node) {
    if (hasFeatureNodeDescriptionElements($node)) {
      $featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';

      // HACK to implement images for taxa, should be removed.
      if ($node->feature->uuid != UUID_IMAGE && $node->feature->uuid != UUID_USE) {
        $countFeatures++;
        $countFeatures++;
        $out .= '<li>' . l(t(theme('cdm_feature_name', array('feature_name' => $featureRepresentation))), $_GET['q'],
            array('attributes' => array('class' => array('toc')),'fragment' => generalizeString($featureRepresentation))) . '</li>';
      }
    }
  }
  // Setting the Anchor to the Bibliography section if the option is enabled.
  $show_bibliography = variable_get('cdm_show_bibliography', 1);

  $markerTypes['markerTypes'] = UUID_MARKERTYPE_USE;
  $useDescriptions = cdm_ws_get(CDM_WS_PORTAL_TAXON_DESCRIPTIONS, substr(strrchr($_GET["q"], '/'), 1), queryString($markerTypes));
  if (!empty($useDescriptions)) {
    $out .= '<li>';
    $out .= l(t(theme('cdm_feature_name', array('feature_name' => 'Uses'))), $_GET['q'], array(
      'attributes' => array('class' => array('toc')),
      'fragment' => 'userecords',
    ));
    $out .= '</li>';
  }

  if ($show_bibliography && $countFeatures != 0) {
    $out .= '<li>' . l(t(theme('cdm_feature_name', array('feature_name' => 'Bibliography'))), $_GET['q'], array('attributes' => array('class' => array('toc')), 'fragment' => 'bibliography')) . '</li>';
  }
  $out .= '</ul>';
  return $out;
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
    $subRank = "infraspecific taxa";
  }
  if ($numberOfChildren != 0) {
    $out .= '<a name="number_of_taxa"> </a><H2>Number of Taxa</H2><div class="content"> <ul class="description">';
    $out .= '<li class=\"descriptionText DescriptionElement\">' . $numberOfChildren . " " . $subRank . '</li></ul>';
  }

  foreach ($mergedFeatureNodes as $node) {

    if (hasFeatureNodeDescriptionElements($node)) {

      $featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';
      $block = new stdClass();
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
        $block->subject = '<span class="' . html_class_atttibute_ref($node->feature) . '">' . theme('cdm_feature_name',  array('feature_name' => $featureRepresentation)) . '</span>';
        $block->module = "cdm_dataportal-feature";
        $block->content = '';

        /*
        Content/DISTRIBUTION.
        */
        if ($node->feature->uuid == UUID_DISTRIBUTION) {

          if (variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0)) {
            $distributionTextDataList = array();
            $distributionElementsList = array();

            foreach ($node->descriptionElements as $descriptionElement) {
              if ($descriptionElement->class == "TextData") {
                $distributionTextDataList[] = $descriptionElement;
              }
              else {
                $distributionElementsList[] = $descriptionElement;
              }
            }
            if (count($distributionTextDataList) > 0) {
              $node->descriptionElements = $distributionElementsList;
              $block->content .= theme('cdm_descriptionElements', array(
                'descriptionElements' => $distributionTextDataList,
                'featureUuid' => $node->feature->uuid,
                'taxon_uuid' => $taxon->uuid,
              ));
            }
          }

          // Display cdm distribution map.
          // TODO this is a HACK to a proper generic implementation?
          $block->content .= theme('cdm_distribution_map', array('taxon' => $taxon));
          $block->content .= theme('cdm_descriptionElements', array(
            'descriptionElements' => $node->descriptionElements,
            'featureUuid' => $node->feature->uuid,
            'taxon_uuid' => $taxon->uuid,
          ));
        }

        /*
        Content/COMMON_NAME.
        */
        elseif ($node->feature->uuid == UUID_COMMON_NAME) {
          // TODO why is theme_cdm_descriptionElement_CommonTaxonName
          // not beeing used???
          $block->content .= theme('cdm_common_names', array('elements' => $node->descriptionElements));
        /*
        }else if($node->feature->uuid == UUID_IMAGE_SOURCES) {
          $block->content .= theme('cdm_image_sources', $node->descriptionElements);
        */
        }

        /*
        Content/ALL OTHER FEATURES.
        */
        elseif ($node->feature->uuid == UUID_USE_RECORD) {
          $block->content .= theme('cdm_block_Uses', $taxon->uuid);
          // $block->content .= theme('cdm_descriptionElements', $node->descriptionElements, $node->feature->uuid, $taxon->uuid),
        }
        else {
          $block->content .= theme('cdm_descriptionElements', array(
            'descriptionElements' => $node->descriptionElements,
            'featureUuid' => $node->feature->uuid,
            'taxon_uuid' => $taxon->uuid,
          ));

          /*
          Content/ALL OTHER FEATURES/Subordinate Features
          subordinate features are printed inline in one floating text,
          it is expected that subordinate features only "contain" TextData
          elements.
          */
          // TODO move into own theme.
          if (count($node->children) > 0) {

            // TODO support more than one level of children.
            // @see http://dev.e-taxonomy.eu/trac/ticket/2393/
            $text = '';
            foreach ($node->children as $child) {
             if (is_array($child->descriptionElements)) {
               foreach ($child->descriptionElements as $element) {

                 if (is_array($element->media)) {
                   // Append media of subordinate elements to the list of
                   // main features.
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
  // Calling the theme function for Bibliography to add it to the output.
  // Add the display of the number of taxa in the selected genus.
  $out .= theme('cdm_block_Uses', array('taxonUuid' => $taxon->uuid));

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
  $query_parameters = $variables['query_parameters'];
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
      'parameters' => $query_parameters,
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
  $fileUri = $variables['fileUri'];

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
 * @todo document this function.
 */
function palmweb_2_cdm_reference($variables) {
  $reference = $variables['reference'];
  $microReference = $variables['microReference'];
  $doLink = $variables['doLink'];
  $referenceStyle = $variables['referenceStyle'];

  $author_team = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $reference->uuid);

  $year = '';
  if (isset($reference->datePublished->start)) {
    $year = partialToYear($reference->datePublished->start);
  }
  $citation = _short_form_of_author_team ($author_team->titleCache) . (!empty($year) ? '. ' . $year : '');
  $citation = str_replace('..', '.', $citation);

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
function palmweb_2_get_partDefinition($variables) {

  if ($variables['nameType'] == 'BotanicalName') {
    return array(
      'namePart' => array('name' => TRUE, 'authors' => TRUE),
      'authorshipPart' => array(),
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
function palmweb_2_get_nameRenderTemplate($variables){
$template = '';

  switch ($variables['renderPath']) {
      case 'acceptedFor':
        $template = array(
          'namePart' => array('#uri'=>TRUE),
        );
        break;
      case 'typedesignations':
        $template = array(
          'namePart' => array('#uri'=>TRUE),
          'referencePart' => TRUE,
        );
      case 'taxon_page_title':
      case 'list_of_taxa':
      case 'taxon_page_synonymy':
      case 'related_taxon':
      case 'polytomousKey':
      case '#DEFAULT':
        $template = array(
          'namePart' => array('#uri'=>TRUE),
          'referencePart' => TRUE,
          'descriptionPart' => TRUE,
          'statusPart' => TRUE,
        );
  }
  return $template;
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
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function palmweb_2_cdm_taxon_page_title($variables){
  $taxon = $variables['taxon'];
  $uuid = $variables['uuid'];
  $synonym_uuid = $variables['synonym_uuid'];

  RenderHints::pushToRenderStack('taxon_page_title');
  $synonym = cdm_ws_get(CDM_WS_PORTAL_TAXON, $synonym_uuid);
  $referenceUri = '';
  if (isset($taxon->name->nomenclaturalReference)) {
    $referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
  }

  $out = theme('cdm_taxonName', array(
    'taxonName' => $taxon->name,
    'nameLink' => NULL,
    'refenceLink' => $referenceUri,
    'show_annotations' => FALSE,
  ));

  RenderHints::popFromRenderStack();
  if (isset($synonym->name->titleCache)) {
  $result = '<span class = "synonym_title">' . $synonym->name->titleCache . ' is synonym of ' . '</span>' .
       '<span class="' . $taxon->class . '">' . $out . '</span>';
  }
  else {
    $result = '<span class="' . $taxon->class . '">' . $out . '</span>';
  }
  return $result;

}

// Comment @WA this theme function does not exist..
/*
function palmweb_2_cdm_uri_to_synonym($synonymUuid, $acceptedUuid, $pagePart = NULL) {
  $acceptedPath = path_to_taxon($acceptedUuid, TRUE);
  return url($acceptedPath . ($pagePart ? '/'.$pagePart : '') . '/'.$synonymUuid, 'highlite='.$synonymUuid);
  //return url($acceptedPath.($pagePart ? '/'.$pagePart : ''), 'highlite='.$synonymUuid, $synonymUuid."/$synonymUuid");
  //return url("$acceptedPath/$synonymUuid".($pagePart ? '/'.$pagePart : ''), 'highlite='.$synonymUuid);
}
*/

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
