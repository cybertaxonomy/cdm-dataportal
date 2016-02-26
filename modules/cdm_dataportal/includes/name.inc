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
 * The render templates arrays contains one or more name render template to be used within the page elements identified the the
 * renderPath. The renderPath is the key of the subelements whereas the value is the name render template.
 * The following keys are curretly recogized:
 *
 *   - list_of_taxa:
 *   - acceptedFor:
 *   - taxon_page_synonymy
 *   - typedesignations
 *   - taxon_page_title
 *   - polytomousKey
 *   - na: name + authorship
 *   - nar:name + authorship + reference
 *   - #DEFAULT
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
 * @param string $renderPath
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
function get_nameRenderTemplate($renderPath, $nameLink = NULL, $referenceLink = NULL) {

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
  if (($separatorPos = strpos($renderPath, '.')) > 0) {
    $renderPath_base = substr($renderPath, 0, $separatorPos);
  } else {
    $renderPath_base = $renderPath;
  }

  $template = NULL;
  // 1. try to find a template using the render path base element
  if(array_key_exists($renderPath_base, $split_render_templates)){
    $template = (array)$split_render_templates[$renderPath_base];
  }

  // 2. Find best matching default RenderTemplate in theme
  // by stripping the dot separated render path element by element
  // is no matching template is found the DEFAULT will be used.
  while (!is_array($template) && strlen($renderPath) > 0) {
    foreach ($split_render_templates as $path => $t) {
      if ($path == $renderPath) {
        $template = $t;
        break;
      }
    }
    // shorten by one element
    $renderPath = substr($renderPath, strrpos($renderPath, '.') + 1, strlen($renderPath));
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
 *
 * @return string
 *  The markup for a taxon name.
 *
 */
function render_taxon_or_name($taxon_name_or_taxon_base, $nameLink = NULL, $refenceLink = NULL,
  $show_annotations = true, $is_type_designation = false, $skiptags = array()) {

  if($taxon_name_or_taxon_base->class == 'Taxon' || $taxon_name_or_taxon_base->class == 'Synonym'){
    $taxonName = $taxon_name_or_taxon_base->name;
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
    $name = '<span class="' . $taxonName->class . '">' . cdm_tagged_text_to_markup($taggedName, 'span', ' ', $skiptags) . '</span>';
  }
  else {
    $name = '<span class="' . $taxonName->class . '_titleCache">' . $taxonName->titleCache . '</span>';
  }

  // Fill name into $renderTemplate.
  array_setr('name', $name, $renderTemplate);

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
  if (isset($nom_status_tagged_text[0])) {
    if (array_setr('status', TRUE, $renderTemplate)) {
      array_setr('status', '<span class="nomenclatural_status">' . cdm_tagged_text_to_markup($nom_status_tagged_text, '', array('postSeparator')) . '</span>', $renderTemplate);
    }
  }

  if (isset($renderTemplate['secReferencePart'])){
    if(isset($secref_tagged_text[1])){
      $post_separator_markup = $is_reference_year ? '.': '';
      if(isset($nom_status_tagged_text[count($nom_status_tagged_text -1 )]) && $nom_status_tagged_text[count($nom_status_tagged_text -1 )]->type ==  'postSeparator'){
        $post_separator_markup = cdm_tagged_text_to_markup(array($nom_status_tagged_text[count($nom_status_tagged_text -1 )]));
      };
      array_setr('secReference',
        $post_separator_markup
          . ' <span class="sec_reference">'
          . $secref_tagged_text[0]->text . $secref_tagged_text[1]->text
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
          $descriptionHtml .= theme("cdm_media", array(
              'descriptionElement' => $description_element,
              'mimeTypePreference' => array(
                'application/pdf',
                'image/png',
                'image/jpeg',
                'image/gif',
                'text/html',
              )
            )
          );

        }
      }
    }
    array_setr('description', $descriptionHtml, $renderTemplate);
  }

  // Render.
  $out = '<span class="' . html_class_attribute_ref($taxon_name_or_taxon_base). '" data-cdm-ref="/name/' . $taxonName->uuid . '">';

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
// =========== START OF HOMONYMS ========== //
  RenderHints::pushToRenderStack('homonym');
  // the render stack element homonyms is being used in the default render templates !!!, see CDM_NAME_RENDER_TEMPLATES_DEFAULT

  // Later homonym or treated as later homonym AND bloking names.
  // TODO apply filter ? $name_rels_to_show = variable_get('name_relationships_to_show', NULL);
  $from_name_relations = cdm_ws_get(CDM_WS_PORTAL_TAXON_FROM_NAMERELATIONS,
    $taxon->uuid);
  $to_name_relations = cdm_ws_get(CDM_WS_PORTAL_TAXON_TO_NAMERELATIONS,
    $taxon->uuid);
  $name_relations = array_merge($from_name_relations, $to_name_relations);

  $homonyms_array = array();

  if ($name_relations) {
    foreach ($name_relations as $element) {
      $taxon_html = NULL;
      switch ($element->type->uuid) {
        case UUID_LATER_HOMONYM :
          $elementTaxonBasesUuid = isset ($element->toName->taxonBases [0]->uuid) ? $element->toName->taxonBases [0]->uuid : '';

          //from relation ships -> only shown at fromName-synonym
          if ($taxon->name->uuid == $element->fromName->uuid) {
            $taxon_html =render_taxon_or_name($element->toName,
              url(path_to_name($element->toName->uuid, $taxon->uuid, $elementTaxonBasesUuid))
            );
          }
          break;
        case UUID_TREATED_AS_LATER_HOMONYM :
          $elementTaxonBasesUuid = isset ($element->toName->taxonBases [0]->uuid) ? $element->toName->taxonBases [0]->uuid : '';

          //from relation ships -> only shown at fromName-synonym
          if ($taxon->name->uuid == $element->fromName->uuid) {
            $taxon_html = render_taxon_or_name($element->toName, url(path_to_name($element->toName->uuid)));
          }
          break;
        case UUID_BLOCKING_NAME_FOR :
          $elementTaxonBasesUuid = isset ($element->fromName->taxonBases [0]->uuid) ? $element->fromName->taxonBases [0]->uuid : '';

          //to relation ships -> only shown at toName-synonym
          if ($taxon->name->uuid == $element->toName->uuid) {
            $taxon_html = render_taxon_or_name($element->fromName,
              url(path_to_name($element->fromName->uuid, $taxon->uuid, $elementTaxonBasesUuid))
            );
          }
          break;
        default:
          $taxon_html = NULL;
      }
      if (isset($taxon_html) && $taxon_html) {
        if (count($homonyms_array)) {
          $homonyms_array [] = 'nec ' . $taxon_html;
        }
        else {
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
