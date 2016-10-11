<?php
/**
 * @file
 * Functions for dealing with CDM entities from the package model.name
 *
 * @copyright
 *   (C) 2007-2015 EDIT
 *   European Distributed Institute of Taxonomy
 *   http://www.e-taxonomy.eu
 *
 *   The contents of this module are subject to the Mozilla
 *   Public License Version 1.1.
 * @see http://www.mozilla.org/MPL/MPL-1.1.html
 *
 * @author
 *   - Andreas Kohlbecker <a.kohlbecker@BGBM.org>
 */

/**
 * @defgroup compose Compose functions
 * @{
 * Functions which are composing Drupal render arays
 *
 * The cdm_dataportal module needs to compose rather complex render arrays from
 * the data returned by the CDM REST service. The compose functions are
 * responsible for creating the render arrays.
 *
 * All these functions are also implementations of the compose_hook()
 * which is used in the proxy_content() function.
 * @}
 */


/**
 * Provides the name render template to be used within the page elements identified the the $renderPath.
 *
 * The render templates arrays contains one or more name render templates to be used within the page elements identified the the
 * renderPath. The renderPath is the key of the subelements whereas the value is the name render template.
 *
 * The render paths used for a cdm_dataportal page can be visualized by supplying the HTTP query parameter RENDER_PATH=1.
 *
 * It will be tried to find  the best matching default RenderTemplate by stripping the dot separated render path
 * element by element. If no matching template is found the DEFAULT will be used:
 *
 * - related_taxon.heterotypicSynonymyGroup.taxon_page_synonymy
 * - related_taxon.heterotypicSynonymyGroup.taxon_page_synonymy
 * - related_taxon.heterotypicSynonymyGroup.taxon_page_synonymy
 *
 * A single render template can be used for multiple render paths. In this case the according key of the render templates
 * array element should be the list of these render paths concatenated by ONLY a comma character without any whitespace.
 *
 * A render template is an associative array. The keys of this array are referring to the keys as defined in the part
 * definitions array.
 * @see get_partDefinition($taxonNameType) for more information
 *
 * The value of the render template element must be set to TRUE in order to let this part being rendered.
 * The namePart, nameAuthorPart and referencePart can also hold an associative array with a single
 * element: array('#uri' => TRUE). The value of the #uri element will be replaced by the according
 * links if the parameters $nameLink or $refenceLink are set.
 *
 * @param string $render_path
 *   The render path can consist of multiple dot separated elements
 *   @see RenderHints::getRenderPath()
 * @param string $nameLink
 *   The link path ot URL to be used for name parts if a link is forseen in the template
 *   matching the given $renderPath.
 * @param string $referenceLink
 *   The link path ot URL to be used for nomenclatural reference parts if a link is forseen
 *   in the template matching the given $renderPath.
 * @return array
 *   An associative array, the render template
 */
function get_nameRenderTemplate($render_path, $nameLink = NULL, $referenceLink = NULL) {

  static $default_render_templates = NULL;
  static $split_render_templates = NULL;


  if (!isset($default_render_templates)) {
    $default_render_templates = unserialize(CDM_NAME_RENDER_TEMPLATES_DEFAULT);
  }
  if($split_render_templates == NULL) {
    $render_templates = variable_get(CDM_NAME_RENDER_TEMPLATES, $default_render_templates);
    // needs to be converted to an array
    $render_templates = (convert_to_array($render_templates));

    // separate render templates which are combined with a comma
    $split_render_templates = array();
    foreach($render_templates as $key => $template){
      if(strpos($key, ',')){
        foreach(explode(',', $key) as $path){
          $split_render_templates[$path] = $template;
        }
      } else {
        $split_render_templates[$key] = $template;
      }
    }
  }

  // get the base element of the renderPath
  if (($separatorPos = strpos($render_path, '.')) > 0) {
    $renderPath_base = substr($render_path, 0, $separatorPos);
  } else {
    $renderPath_base = $render_path;
  }

  $template = NULL;
  // 1. try to find a template using the render path base element
  if(array_key_exists($renderPath_base, $split_render_templates)){
    $template = (array)$split_render_templates[$renderPath_base];
  }

  // 2. Find best matching default RenderTemplate
  // by stripping the dot separated render path element by element
  // if no matching template is found the DEFAULT will be used.
  while (!is_array($template) && strlen($render_path) > 0) {
    foreach ($split_render_templates as $path => $t) {
      if ($path == $render_path) {
        $template = $t;
        break;
      }
    }
    // shorten by one element
    $render_path = substr($render_path, strrpos($render_path, '.') + 1, strlen($render_path));
  }


  // 3. Otherwise get default RenderTemplate from theme.
  if (!is_array($template)) {
    $template = $split_render_templates['#DEFAULT'];
  }

  // --- set the link uris to the according template fields if they exist
  if(isset($template['nameAuthorPart']) && isset($template['nameAuthorPart']['#uri'])) {
    if ($nameLink) {
      $template['nameAuthorPart']['#uri'] = $nameLink;
    }
    else {
      unset($template['nameAuthorPart']['#uri']);
    }
  }

  if ($nameLink && isset($template['namePart']['#uri'])) {
    $template['namePart']['#uri'] = $nameLink;
  }
  else {
    unset($template['namePart']['#uri']);
  }

  if ($referenceLink && isset($template['referencePart']['#uri'])) {
    $template['referencePart']['#uri'] = $referenceLink;
  }
  else {
    unset($template['referencePart']['#uri']);
  }

  return $template;
}

/**
 * The part definitions define the specific parts of which a rendered taxon name plus additional information will consist.
 *
 * A full taxon name plus additional information can consist of the following elements:
 *
 *   - name: the taxon name inclugin rank nbut without author
 *   - authorTeam:  The authors of a reference, also used in taxon names
 *   - authors:  The authors of a reference, also used in taxon names
 *   - reference: the nomenclatural reference,
 *   - microreference:  Volume, page number etc.
 *   - status:  The nomenclatural status of a name
 *   - description: name descriptions like protologues etc ...
 *
 * These elements are combined in the part definitions array to from the specific parts to be rendered.
 * Usually the following parts are formed:
 *
 * The name "Lapsana communis L., Sp. Pl.: 811. 1753" shall be an example here:
 *  - namePart: the name and rank (in example: "Lapsana communis")
 *  - authorshipPart: the author (in example: "L.")
 *  - nameAuthorPart: the combination of name and author part (in example: "Lapsana communis L.").
 *     This is useful for zoological names where the authorshipPart belongs to the name and both should
 *     be combined when a link to the taxon is rendered.
 *  - referencePart: the nomencaltural reference (in example: "Sp. Pl. 1753")
 *  - microreferencePart: usually the page number (in example ": 811.")
 *  - statusPart: the nomenclatorical status
 *  - descriptionPart:
 *
 * Each set of parts is dedicated to render a specific TaxonName type, the type names are used as keys for the
 * specific parts part definitions:
 *
 *  - BotanicalName
 *  - ZoologicalName
 *  - #DEFAULT:  covers ViralNames and general NonViralNames
 *
 * An example:
 * @code
 * array(
 *    'ZoologicalName' => array(
 *        'namePart' => array('name' => TRUE),
 *        'referencePart' => array('authorTeam' => TRUE),
 *        'microreferencePart' => array('microreference' => TRUE),
 *        'statusPart' => array('status' => TRUE),
 *        'descriptionPart' => array('description' => TRUE),
 *    ),
 *    'BotanicalName' => array(
 *        'namePart' => array(
 *            'name' => TRUE,
 *            'authors' => TRUE,
 *        ),
 *        'referencePart' => array(
 *            'reference' => TRUE,
 *            'microreference' => TRUE,
 *        ),
 *        'statusPart' => array('status' => TRUE),
 *        'descriptionPart' => array('description' => TRUE),
 *    ),
 *  );
 * @endcode
 *
 */
function get_partDefinition($taxonNameType) {

  static $default_part_definitions = null;
  if (!isset($default_part_definitions)) {
    $default_part_definitions= unserialize(CDM_PART_DEFINITIONS_DEFAULT);
  }

  static $part_definitions = null;
  if (!isset($part_definitions)) {
    $part_definitions = convert_to_array(variable_get(CDM_PART_DEFINITIONS, $default_part_definitions));
  }

  if (array_key_exists($taxonNameType, $part_definitions)) {
    return $part_definitions[$taxonNameType];
  } else {
    return $part_definitions['#DEFAULT']; // covers ViralNames and general NonViralNames
  }

}


/**
 * Renders the markup for a CDM TaxonName instance.
 *
 * The layout of the name representation is configured by the
 * part_definitions and render_templates (see get_partDefinition() and
 * get_nameRenderTemplate())
 *
 * @param $taxonName
 *    cdm TaxonNameBase instance
 * @param $sec
 *    the sec reference of a taxon having this name (optional)
 * @param $nameLink
 *    URI to the taxon, @see path_to_taxon(), must be processed by url() before passing to this method
 * @param $refenceLink
 *    URI to the reference, @see path_to_reference(), must be processed by url() before passing to this method
 * @param $show_annotations
 *    turns the display of annotations on
 * @param $is_type_designation
 *    To indicate that the supplied taxon name is a name type designation.
 * @param $skiptags
 *    an array of name elements tags like 'name', 'rank' to skip. The name part
 *          'authors' will not ber affected by this filter. This part is managed though the render template
 *          mechanism.
 * @param $is_invalid
 *   Indicates that this taxon is invalid. In this case the name part will be shown in double quotes.
 *   This is useful when rendering taxon relation ships.
 *
 * @return string
 *  The markup for a taxon name.
 *
 */
function render_taxon_or_name($taxon_name_or_taxon_base, $nameLink = NULL, $refenceLink = NULL,
  $show_annotations = true, $is_type_designation = false, $skiptags = array(), $is_invalid = false) {

  $is_doubtful = false;

  if($taxon_name_or_taxon_base->class == 'Taxon' || $taxon_name_or_taxon_base->class == 'Synonym'){
    $taxonName = $taxon_name_or_taxon_base->name;
    $is_doubtful = $taxon_name_or_taxon_base->doubtful;
    // use the TaxonBase.taggedTitle so we have the secRef
    $taggedTitle = $taxon_name_or_taxon_base->taggedTitle;
  } else {
    // assuming this is a TaxonNameBase
    $taxonName = $taxon_name_or_taxon_base;
    $taggedTitle = $taxon_name_or_taxon_base->taggedName;
  }


  $renderTemplate = get_nameRenderTemplate(RenderHints::getRenderPath(), $nameLink, $refenceLink);
  $partDefinition = get_partDefinition($taxonName->class);

  // Apply definitions to template.
  foreach ($renderTemplate as $part => $uri) {

    if (isset($partDefinition[$part])) {
      $renderTemplate[$part] = $partDefinition[$part];
    }
    if (is_array($uri) && isset($uri['#uri'])) {
      $renderTemplate[$part]['#uri'] = $uri['#uri'];
    }
  }

  $secref_tagged_text = split_secref_from_tagged_text($taggedTitle);
  $nom_status_tagged_text = split_nomstatus_from_tagged_text($taggedTitle);
  $appended_phrase_tagged_text = array(); // this is filled later

  normalize_tagged_text($taggedTitle);

  $firstEntryIsValidNamePart =
    isset($taggedTitle)
    && is_array($taggedTitle)
    && isset($taggedTitle[0]->text)
    && is_string($taggedTitle[0]->text)
    && $taggedTitle[0]->text != ''
    && isset($taggedTitle[0]->type)
    && $taggedTitle[0]->type == 'name';
  $lastAuthorElementString = FALSE;

  $name_encasement = $is_invalid ? '"' : '';
  $doubtful_marker = $is_doubtful ? '?&#8239;' : ''; // 	&#8239; =  NARROW NO-BREAK SPACE
  $doubtful_marker_markup = '';

  if($doubtful_marker){
    $doubtful_marker_markup = '<span class="doubtful">' . $doubtful_marker . '</span>';
  }

  // split off all appendedPhrase item  from the end of the array (usually there only should  be one)
  while($taggedTitle[count($taggedTitle)-1]->type == "appendedPhrase"){
    $appended_phrase_tagged_text[] = array_pop($taggedTitle);
  }

  // Got to use second entry as first one, see ToDo comment below ...
  if ($firstEntryIsValidNamePart) {

    $taggedName = $taggedTitle;
    $hasNamePart_with_Authors = isset($renderTemplate['namePart']) && isset($renderTemplate['namePart']['authors']);
    $hasNameAuthorPart_with_Authors = isset($renderTemplate['nameAuthorPart']) && isset($renderTemplate['nameAuthorPart']['authors']);


    if (!(($hasNamePart_with_Authors) || ($hasNameAuthorPart_with_Authors))) {
      // Find author and split off from name.
      // TODO expecting to find the author as the last element.
      /*
      if($taggedName[count($taggedName)- 1]->type == 'authors'){
        $authorTeam = $taggedName[count($taggedName)- 1]->text;
        unset($taggedName[count($taggedName)- 1]);
      }
      */

      // Remove all authors.
      $taggedNameNew = array();
      foreach ($taggedName as $element) {
        if ($element->type != 'authors') {
          $taggedNameNew[] = $element;
        }
        else {
          $lastAuthorElementString = $element->text;
        }
      }
      $taggedName = $taggedNameNew;
      unset($taggedNameNew);
    }
    $name = '<span class="' . $taxonName->class . '">' . $doubtful_marker_markup . $name_encasement . cdm_tagged_text_to_markup($taggedName, 'span', ' ', $skiptags) . $name_encasement . '</span>';
  }
  else {
    $name = '<span class="' . $taxonName->class . '_titleCache">' . $doubtful_marker_markup . $name_encasement . $taxonName->titleCache . $name_encasement . '</span>';
  }


  if(isset($appended_phrase_tagged_text[0])){
    $name .= ' <span class="appended-phrase">'. cdm_tagged_text_to_markup($appended_phrase_tagged_text) . '</span>';
  }

  // Fill name into $renderTemplate.
  array_setr('name', $name , $renderTemplate);

  // Fill with authorTeam.
  /*
  if($authorTeam){
    $authorTeamHtml = ' <span class="authorTeam">'.$authorTeam.'</span>';
    array_setr('authorTeam', $authorTeamHtml, $renderTemplate);
  }
  */

  // Fill with reference.
  if (isset($renderTemplate['referencePart']) && !$is_type_designation) {

    // default separator
    $separator = '';

    // [Eckhard]:"Komma nach dem Taxonnamen ist grunsätzlich falsch,
    // Komma nach dem Autornamen ist überall dort falsch, wo ein "in" folgt."
    if (isset($renderTemplate['referencePart']['reference']) && isset($taxonName->nomenclaturalReference)) {
      $microreference = NULL;
      if (isset($renderTemplate['referencePart']['microreference'])&& isset($taxonName->nomenclaturalMicroReference)) {
        $microreference = $taxonName->nomenclaturalMicroReference;
      }
      $citation = cdm_ws_getNomenclaturalReference($taxonName->nomenclaturalReference->uuid, $microreference);

      // Find preceding element of the reference.
      $precedingKey = get_preceding_contentElementKey('reference', $renderTemplate);
      if (str_beginsWith($citation, ", in")) {
        $citation = substr($citation, 2);
        $separator = ' ';
      }
      elseif (!str_beginsWith($citation, "in") && $precedingKey == 'authors') {
        $separator = ', ';
      } else {
        $separator = ' ';
      }


      $referenceArray['#separator'] = $separator;
      $referenceArray['#html'] = '<span class="reference">' . $citation . '</span>';
      array_setr('reference', $referenceArray, $renderTemplate);
    }

    // If authors have been removed from the name part the last named authorteam
    // should be added to the reference citation, otherwise, keep the separator
    // out of the reference.
    if (isset($renderTemplate['referencePart']['authors']) && $lastAuthorElementString) {
      // If the nomenclaturalReference citation is not included in the
      // reference part but diplay of the microreference
      // is wanted, append the microreference to the authorTeam.
      $citation = '';
      if (!isset($renderTemplate['referencePart']['reference']) && isset($renderTemplate['referencePart']['microreference'])) {
        $separator = ": ";
        $citation = $taxonName->nomenclaturalMicroReference;
      }
      $referenceArray['#html'] = ' <span class="reference">' . $lastAuthorElementString . $separator . $citation . '</span>';
      array_setr('authors', $referenceArray, $renderTemplate);
    }
  }

  $is_reference_year = false;
  if (isset($renderTemplate['referenceYearPart']['reference.year'])) {
    if(isset($taxonName->nomenclaturalReference->datePublished)){
      $referenceArray['#html'] = ' <span class="reference">' . timePeriodToString($taxonName->nomenclaturalReference->datePublished) . '</span>';
      array_setr('reference.year', $referenceArray, $renderTemplate);
      $is_reference_year = true;
    }
  }

  // Fill with status.
  if(isset($renderTemplate['statusPart']['status'])){
    if (isset($nom_status_tagged_text[0])) {
        array_setr('status', '<span class="nomenclatural_status">' . cdm_tagged_text_to_markup($nom_status_tagged_text, 'span', '', array('postSeparator')) . '</span>', $renderTemplate);
    }
  }

  if (isset($renderTemplate['secReferencePart'])){
    if(isset($secref_tagged_text[1])){
      $post_separator_markup = $is_reference_year ? '.': '';
      if(isset($nom_status_tagged_text[count($nom_status_tagged_text) - 1]) && ($nom_status_tagged_text[count($nom_status_tagged_text) - 1]->type ==  'postSeparator')){
        $post_separator_markup = cdm_tagged_text_to_markup(array($nom_status_tagged_text[count($nom_status_tagged_text) - 1 ]));
      };
      array_setr('secReference',
        $post_separator_markup
          . ' <span class="sec_reference">'
          . join('', cdm_tagged_text_values($secref_tagged_text))
          . '</span>', $renderTemplate);
    }
  }

  // Fill with protologues etc...
  $descriptionHtml = '';
  if (array_setr('description', TRUE, $renderTemplate)) {
    $descriptions = cdm_ws_get(CDM_WS_PORTAL_NAME_DESCRIPTIONS, $taxonName->uuid);
    foreach ($descriptions as $description) {
      if (!empty($description)) {
        foreach ($description->elements as $description_element) {
          $second_citation = '';
          if (isset($description_element->multilanguageText_L10n) && $description_element->multilanguageText_L10n->text) {
            $second_citation = '[& ' . $description_element->multilanguageText_L10n->text . '].';
          }
          $descriptionHtml .= $second_citation;
          $descriptionHtml .= cdm_description_element_media(
              $description_element,
              array(
                'application/pdf',
                'image/png',
                'image/jpeg',
                'image/gif',
                'text/html',
              )
          );

        }
      }
    }
    array_setr('description', $descriptionHtml, $renderTemplate);
  }

  // Render.
  $out = '';
  if(isset($_REQUEST['RENDER_PATH'])){
    // developer option to show the render path with each taxon name
    $out .= '<span class="render-path">' . RenderHints::getRenderPath() . '</span>';
  }
  $out .= '<span class="' . html_class_attribute_ref($taxon_name_or_taxon_base)
    . '" data-cdm-ref="/name/' . $taxonName->uuid . '" data-cdm-render-path="' . RenderHints::getRenderPath() .'">';

  foreach ($renderTemplate as $partName => $part) {
    $separator = '';
    $partHtml = '';
    $uri = FALSE;
    if (!is_array($part)) {
      continue;
    }
    if (isset($part['#uri']) && is_string($part['#uri'])) {
      $uri = $part['#uri'];
      unset($part['#uri']);
    }
    foreach ($part as $key => $content) {
      $html = '';
      if (is_array($content)) {
        $html = $content['#html'];
        if(isset($content['#separator'])) {
          $separator = $content['#separator'];
        }
      }
      elseif (is_string($content)) {
        $html = $content;
      }
      $partHtml .= '<span class="' . $key . '">' . $html . '</span>';
    }
    if ($uri) {
      // cannot use l() here since the #uri aleady should have been processed through uri() at this point
      $out .= $separator . '<a href="' . $uri . '" class="' . $partName . '">' . $partHtml . '</a>';

    }
    else {
      $out .= $separator . $partHtml;
    }
  }
  $out .= '</span>';
  if ($show_annotations) {
    // $out .= theme('cdm_annotations_as_footnotekeys', $taxonName);
  }
  return $out;
}

/**
 * Renders the string of Homonyms for a given taxon.
 *
 * @param $taxon
 *    A CDM Taxon instance
 * @return String
 *    The string of homomyns
 *
 * @throws \Exception
 */
function cdm_name_relationships_of($taxon) {

  static $inverse_name_rels_uuids = array(UUID_NAMERELATIONSHIPTYPE_BLOCKING_NAME_FOR);

  // =========== START OF HOMONYMS ========== //
  RenderHints::pushToRenderStack('homonym');
  // the render stack element homonyms is being used in the default render templates !!!, see CDM_NAME_RENDER_TEMPLATES_DEFAULT

  $selected_name_rel_uuids = variable_get(CDM_NAME_RELATIONSHIP_TYPES, unserialize(CDM_NAME_RELATIONSHIP_TYPES_DEFAULT));

  $from_name_relations = cdm_ws_get(CDM_WS_PORTAL_TAXON_FROM_NAMERELATIONS,
    $taxon->uuid);
  $to_name_relations = cdm_ws_get(CDM_WS_PORTAL_TAXON_TO_NAMERELATIONS,
    $taxon->uuid);
  $name_relations = array_merge($from_name_relations, $to_name_relations);

  $homonyms_array = array();

  if ($name_relations) {

    foreach ($name_relations as $element) {

      if(!(isset($selected_name_rel_uuids[$element->type->uuid]) && $selected_name_rel_uuids[$element->type->uuid])){
        // skip if not selected in the settings
        continue;
      }
      $taxon_html = null;
      if(array_search($element->type->uuid, $inverse_name_rels_uuids) !== false) {
        // case of UUID_NAMERELATIONSHIPTYPE_BLOCKING_NAME_FOR
        //to relation ships -> only shown at toName-synonym
        $elementTaxonBasesUuid = isset ($element->fromName->taxonBases [0]->uuid) ? $element->fromName->taxonBases [0]->uuid : '';
        if ($taxon->name->uuid == $element->toName->uuid) {
          $taxon_html = render_taxon_or_name($element->fromName,
            url(path_to_name($element->fromName->uuid, $taxon->uuid, $elementTaxonBasesUuid))
          );
        }

      } else {
        $elementTaxonBasesUuid = isset ($element->toName->taxonBases [0]->uuid) ? $element->toName->taxonBases [0]->uuid : '';
        //from relation ships -> only shown at fromName-synonym
        if ($taxon->name->uuid == $element->fromName->uuid) {
          $taxon_html = render_taxon_or_name($element->toName,
            url(path_to_name($element->toName->uuid, $taxon->uuid, $elementTaxonBasesUuid))
          );
        }
      }

      if($taxon_html){
        if (count($homonyms_array)) {
          // lat: "non nec" == german: "weder noch"
          $homonyms_array [] = 'nec ' . $taxon_html;
        } else {
          $homonyms_array [] = 'non ' . $taxon_html;
        }
      }

    }
  }

  RenderHints::popFromRenderStack();
  return (count($homonyms_array) ?'[' . trim(join(" ", $homonyms_array)) . ']' : '');
}


  /**
 * Recursively searches the array for the $key and sets the given value.
 *
 * @param mixed $key
 *   Key to search for.
 * @param mixed $value
 *   Value to set.'
 * @param array $array
 *   Array to search in.
 *
 * @return bool
 *   True if the key has been found.
 */
function &array_setr($key, $value, array &$array) {
  $res = NULL;
  foreach ($array as $k => &$v) {
    if ($key == $k) {
      $v = $value;
      return $array;
    }
    elseif (is_array($v)) {
      $innerArray = array_setr($key, $value, $v);
      if ($innerArray) {
        return $array;
      }
    }
  }
  return $res;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function &get_preceding_contentElement($contentElementKey, array &$renderTemplate) {
  $res = NULL;
  $precedingElement = NULL;
  foreach ($renderTemplate as &$part) {
    foreach ($part as $key => &$element) {
      if ($key == $contentElementKey) {
        return $precedingElement;
      }
      $precedingElement = $element;
    }
  }
  return $res;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function &get_preceding_contentElementKey($contentElementKey, array &$renderTemplate) {
  $res = NULL;
  $precedingKey = NULL;
  foreach ($renderTemplate as &$part) {
    if (is_array($part)) {
      foreach ($part as $key => &$element) {
        if ($key == $contentElementKey) {
          return $precedingKey;
        }
        if (!str_beginsWith($key, '#')) {
          $precedingKey = $key;
        }
      }
    }
  }
  return $res;
}
