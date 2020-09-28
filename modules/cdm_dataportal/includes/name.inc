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
 *   The link path or URL to be used for name parts if a link is forseen in the template
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
    $render_templates = (object_to_array($render_templates));

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
 * @param object $taxonNameType
 *    A cdm TaxonNameType entity
 *
 */
function get_partDefinition($taxonNameType) {

  static $default_part_definitions = null;
  if (!isset($default_part_definitions)) {
    $default_part_definitions= unserialize(CDM_PART_DEFINITIONS_DEFAULT);
  }

  static $part_definitions = null;
  if (!isset($part_definitions)) {
    $part_definitions = object_to_array(variable_get(CDM_PART_DEFINITIONS, $default_part_definitions));
  }

  $dtype = nameTypeToDTYPE($taxonNameType);
  if (array_key_exists($taxonNameType, $part_definitions)) {
    return $part_definitions[$taxonNameType];
  } else if (array_key_exists($dtype, $part_definitions)) {
    return $part_definitions[$dtype];
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
 * @param $taxon_name_or_taxon_base
 *    A cdm TaxonBase or TaxonName entity
 * @param $name_link
 *    URI to the taxon, @param $reference_link
 *    URI to the reference,
 * @param bool $show_annotations
 *    turns the display of annotations on
 * @param bool $is_type_designation
 *    To indicate that the supplied taxon name is a name type designation.
 * @param array $skiptags
 *    an array of name elements tags like 'name', 'rank' to skip. The name part
 *          'authors' will not ber affected by this filter. This part is managed though the render template
 *          mechanism.
 * @param bool $is_invalid
 *   Indicates that this taxon is invalid. In this case the name part will be shown in double quotes.
 *   This is useful when rendering taxon relation ships.
 *
 * @return string
 *  The markup for a taxon name.
 * @see path_to_taxon(), must be processed by url() before passing to this method
 * @see path_to_reference(), must be processed by url() before passing to this method
 */
function render_taxon_or_name($taxon_name_or_taxon_base, $name_link = NULL, $reference_link = NULL,
  $show_annotations = true, $is_type_designation = false, $skiptags = array(), $is_invalid = false) {

  $is_doubtful = false;
  $taxon_base = null;
  if($taxon_name_or_taxon_base->class == 'Taxon' || $taxon_name_or_taxon_base->class == 'Synonym'){
    $taxon_base = $taxon_name_or_taxon_base;
    if(isset($taxon_name_or_taxon_base->name)){
      $taxon_name = $taxon_name_or_taxon_base->name;
    } else {
      $taxon_name = cdm_ws_get(CDM_WS_TAXON . '/$0/name', array($taxon_name_or_taxon_base->uuid));
    }
    $is_doubtful = $taxon_name_or_taxon_base->doubtful;
    // use the TaxonBase.tagged_title so we have the secRef
    $tagged_title = $taxon_name_or_taxon_base->taggedTitle;
  } else {
    // assuming this is a TaxonName
    $taxon_name = $taxon_name_or_taxon_base;
    if(isset($taxon_name->taggedFullTitle)){
      $tagged_title = $taxon_name_or_taxon_base->taggedFullTitle;
    } else {
      $tagged_title = $taxon_name_or_taxon_base->taggedName;
    }
  }


  $renderTemplate = get_nameRenderTemplate(RenderHints::getRenderPath(), $name_link, $reference_link);
  $partDefinition = get_partDefinition($taxon_name->nameType);

  // Apply definitions to template.
  foreach ($renderTemplate as $part => $uri) {

    if (isset($partDefinition[$part])) {
      $renderTemplate[$part] = $partDefinition[$part];
    }
    if (is_array($uri) && isset($uri['#uri'])) {
      $renderTemplate[$part]['#uri'] = $uri['#uri'];
    }
  }

  $secref_tagged_text = tagged_text_extract_reference_and_detail($tagged_title);
  // taxon names will have the nomenclatural reference in the tagged full title:
  $nomref_tagged_text = tagged_text_extract_reference($tagged_title);
  $nom_status_tagged_text = tagged_text_extract_nomstatus($tagged_title);
  $appended_phrase_tagged_text = array(); // this is filled later

  normalize_tagged_text($tagged_title);

  $is_valid_tagged_title =
    isset($tagged_title)
    && is_array($tagged_title)
    && isset($tagged_title[0]->text)
    && is_string($tagged_title[0]->text)
    && $tagged_title[0]->text != ''
    && isset($tagged_title[0]->type);
  $lastAuthorElementString = FALSE;

  $name_encasement = $is_invalid ? '"' : '';
  $doubtful_marker = $is_doubtful ? '?&#8239;' : ''; // 	&#8239; =  NARROW NO-BREAK SPACE
  $doubtful_marker_markup = '';

  if($doubtful_marker){
    $doubtful_marker_markup = '<span class="doubtful">' . $doubtful_marker . '</span>';
    if($tagged_title[0]->text == '?' ){
      // remove the first tagged text element
      unset($tagged_title[0]);
    }
  }

  // split off all appendedPhrase item  from the end of the array (usually there only should  be one)
  while($tagged_title[count($tagged_title)-1]->type == "appendedPhrase"){
    $appended_phrase_tagged_text[] = array_pop($tagged_title);
  }

  // Got to use second entry as first one, see ToDo comment below ...
  if ($is_valid_tagged_title) {

    $taggedName = $tagged_title;
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
    $name = '<span class="' . $taxon_name->class . '">' . $doubtful_marker_markup . $name_encasement . cdm_tagged_text_to_markup($taggedName, $skiptags) . $name_encasement . '</span>';
  }
  else {
    // use titleCache instead
    $name = '<span class="' . $taxon_name->class . '_titleCache">' . $doubtful_marker_markup . $name_encasement . $taxon_name->titleCache . $name_encasement . '</span>';
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

    $registrations = cdm_ws_get(CDM_WS_NAME, array($taxon_name->uuid, "registrations"));
    $registration_markup = render_registrations($registrations);

    // default separator
    $separator = '';

    // [Eckhard]:"Komma nach dem Taxonnamen ist grunsätzlich falsch,
    // Komma nach dem Autornamen ist überall dort falsch, wo ein "in" folgt."
    if (isset($renderTemplate['referencePart']['reference'])) {
      $microreference = NULL;
      if (isset($renderTemplate['referencePart']['microreference'])&& isset($taxon_name->nomenclaturalSource->citationMicroReference)) {
        $microreference = $taxon_name->nomenclaturalSource->citationMicroReference;
      }
      if(count($nomref_tagged_text) == 0 && isset($taxon_name->nomenclaturalSource->citation)){
        // TODO is this case still relevant? The tagged text should already contain all information!
        $citation = cdm_ws_getNomenclaturalReference($taxon_name->nomenclaturalSource->citation->uuid, $microreference);
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
        $referenceArray['#html'] = '<span class="reference">' . $citation . '</span>' . $registration_markup;
      } else {
        // this ist the case for taxon names
        $referenceArray['#html'] = cdm_tagged_text_to_markup($nomref_tagged_text);
      }


      array_setr('reference', $referenceArray, $renderTemplate);
    }

    // If authors have been removed from the name part the last named authorteam
    // should be added to the reference citation, otherwise, keep the separator
    // out of the reference.
    if (isset($renderTemplate['referencePart']['authors']) && $lastAuthorElementString) {
      // If the nomenclaturalReference citation is not included in the
      // reference part but display of the microreference
      // is wanted, append the microreference to the authorTeam.
      $citation = '';
      if (!isset($renderTemplate['referencePart']['reference']) && isset($renderTemplate['referencePart']['microreference'])) {
        $separator = ": ";
        $citation = $taxon_name->nomenclaturalMicroReference;
      }
      $referenceArray['#html'] = ' <span class="reference">' . $lastAuthorElementString . $separator . $citation . '</span>';
      array_setr('authors', $referenceArray, $renderTemplate);
    }
  }

  $is_reference_year = false;
  if (isset($renderTemplate['referenceYearPart']['reference.year'])) {
    if(isset($taxon_name->nomenclaturalSource->citation->datePublished)){
      $referenceArray['#html'] = ' <span class="reference">' . timePeriodToString($taxon_name->nomenclaturalSource->citation->datePublished) . '</span>';
      array_setr('reference.year', $referenceArray, $renderTemplate);
      $is_reference_year = true;
    }
  }

  // Fill with status.
  if(isset($renderTemplate['statusPart']['status'])){
    if (isset($nom_status_tagged_text[0])) {
        $tt_to_markup_options = array('html' => false);
        foreach ($nom_status_tagged_text as &$tt){
         if($tt->type == 'nomStatus'&& isset($tt->entityReference)) {
           $nom_status = cdm_ws_get(CDM_WS_NOMENCLATURALSTATUS, array($tt->entityReference->uuid));
           $nom_status_fkey = handle_nomenclatural_status_as_footnote($nom_status);
           $tt->text .= $nom_status_fkey;
           $tt_to_markup_options['html'] = true;
         }
        }
        array_setr(
          'status',
          '<span class="nomenclatural_status">' . cdm_tagged_text_to_markup($nom_status_tagged_text, array('postSeparator'), 'span', $tt_to_markup_options) . '</span>',
          $renderTemplate);
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
    $descriptions = cdm_ws_get(CDM_WS_PORTAL_NAME_DESCRIPTIONS, $taxon_name->uuid);
    if($descriptions){
      foreach ($descriptions as $description) {
        if (!empty($description)) {
          foreach ($description->elements as $description_element) {
            $second_citation = '';
            if (isset($description_element->multilanguageText_L10n) && $description_element->multilanguageText_L10n->text) {
              if(isset($description_element->feature) && $description_element->feature->uuid == UUID_ADDITIONAL_PUBLICATION){
                $prefix =  '& ';
              } else {
                $prefix = '';
              }
              $second_citation = ' [' . $prefix . $description_element->multilanguageText_L10n->text . '].';
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
    . '" data-cdm-ref="/name/' . $taxon_name->uuid . '" data-cdm-render-path="' . RenderHints::getRenderPath() .'">';

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
    $footnote_keys = [];
    if($taxon_base){
      $annotations_and_sources_taxon = handle_annotations_and_sources($taxon_base);
      $footnote_keys[] = $annotations_and_sources_taxon['foot_note_keys'];
    }
    $annotations_and_sources_name = handle_annotations_and_sources($taxon_name);
    $footnote_keys[] = $annotations_and_sources_name['foot_note_keys'];
    // FIXME the glue should be inside of the <span class="footnote-key ... DOM
    // element, see render_footnote_key()
    $out .= implode(',', array_filter($footnote_keys, 'strlen'));
  }
  return $out;
}



/**
 * Composes information for a registration from a dto object.
 *
 * Registrations which are not yet published are suppressed.
 *
 * @param $registration_dto
 * @param $with_citation
 *   Whether to show the citation.
 *
 * @return array
 *    A drupal render array with the elements:
 *    - 'name'
 *    - 'name-relations'
 *    - 'specimen_type_designations'
 *    - 'name_type_designations'
 *    - 'citation'
 *    - 'registration_date_and_institute'
 * @ingroup compose
 */
function compose_registration_dto_full($registration_dto, $with_citation = true)
{
  $render_array = array(
    '#prefix' => '<div class="registration">',
    '#suffix' => '</div>'
  );

  if(!(isset($registration_dto->identifier) && $registration_dto->status == 'PUBLISHED')){
    return $render_array;
  }

  $render_array['sub_headline'] = markup_to_render_array(join(", ", registration_types($registration_dto)),-10, '<h3 class="registration_type">' . t('Event: '), '</h3>' );
  $render_array['nomenclatural_act'] = array(
    '#weight' => 0,
    '#prefix' => '<div class="nomenclatural_act">',

    '#suffix' => '</div>'
  );

  $typified_name = null;

  // Nomenclatural act block element
  $last_footnote_listkey = RenderHints::setFootnoteListKey("nomenclatural_act");
  // name
  $name_relations = null;
  if($registration_dto->nameRef){
    $name = cdm_ws_get(CDM_WS_PORTAL_NAME, $registration_dto->nameRef->uuid);
    cdm_load_tagged_full_title($name);
    $render_array['nomenclatural_act']['published_name'] = markup_to_render_array('<div class="published-name">' . render_taxon_or_name($name, url(path_to_name($name->uuid))) . '</div>', 0);
    $name_relations = cdm_ws_fetch_all(str_replace("$0", $registration_dto->nameRef->uuid, CDM_WS_PORTAL_NAME_NAME_RELATIONS));
    // need to create the name relationships later, so that the foot notes are in correct order, see section // name relations
  } else {
    // in this case the registration must have a
    // typified name will be rendered later
    $typified_name = cdm_ws_get(CDM_WS_PORTAL_NAME, $registration_dto->typifiedNameRef->uuid);

  }

  // typedesignation in detail
  if(is_object($registration_dto->orderdTypeDesignationWorkingSets)) {
    $field_unit_uuids = array();
    $specimen_type_designation_refs = array();
    $name_type_designation_refs = array();
    foreach ((array)$registration_dto->orderdTypeDesignationWorkingSets as $workingset_ref => $obj) {
      $tokens = explode("#", $workingset_ref);
      $types_in_fieldunit = get_object_vars($obj); // convert into associative array

      if ($tokens[0] == 'NameTypeDesignation') {
        foreach ($types_in_fieldunit as $type_status => $entity_reference_list) {
          if(!isset($name_type_designation_refs[$type_status])){
            $name_type_designation_refs[$type_status]  = $entity_reference_list;
          } else {
            array_push($name_type_designation_refs[$type_status] ,$entity_reference_list);
          }
        }
      } else if ($tokens[0] == 'FieldUnit'){
        $field_unit_uuids[] = $tokens[1];
        foreach ($types_in_fieldunit as $type_status => $entity_reference_list) {
          if(!isset($specimen_type_designation_refs[$type_status])){
            $specimen_type_designation_refs[$type_status] =  $entity_reference_list;
          } else {
            array_push($specimen_type_designation_refs[$type_status], $entity_reference_list);
          }
        }
      } else {
        drupal_set_message("Unimplemented type: " . $tokens[0], 'error');
      }
    }
    // type designations which are in this nomenclatural act.
    if (count($name_type_designation_refs) > 0) {
      $render_array['nomenclatural_act']['name_type_designations'] = compose_name_type_designations($name_type_designation_refs);
      $render_array['nomenclatural_act']['name_type_designations']['#prefix'] = '<p class="name_type_designations">';
      $render_array['nomenclatural_act']['name_type_designations']['#suffix'] = '</p>';
      $render_array['nomenclatural_act']['name_type_designations']['#weight'] = 20;
    }
    if (count($field_unit_uuids) > 0) {
      $specimen_type_designations_array = compose_specimen_type_designations($specimen_type_designation_refs, true);
      $render_array['nomenclatural_act']['specimen_type_designations'] = $specimen_type_designations_array['type_designations'];
      $render_array['map'] = $specimen_type_designations_array['map'];
      $render_array['map']['#weight'] = $render_array['nomenclatural_act']['#weight'] + 20;
    }
  }

  // name relations
  if($name_relations){
    $render_array['nomenclatural_act']['name_relations'] = compose_name_relationships_list($name_relations, $registration_dto->nameRef->uuid, null);
    $render_array['nomenclatural_act']['name_relations']['#weight'] = 10;
  }

  // citation
  if ($with_citation) {
    $render_array['citation'] = markup_to_render_array(
      "<div class=\"citation nomenclatural_act_citation" . html_class_attribute_ref(new TypedEntityReference("Reference", $registration_dto->citationUuid)) . "\">"
      . "<span class=\"label\">published in: </span>"
      . $registration_dto->bibliographicInRefCitationString
      . l(custom_icon_font_markup('icon-interal-link-alt-solid', array('class' => array('superscript'))), path_to_reference($registration_dto->citationUuid), array('html' => true))
      . "</div>",
      $render_array['nomenclatural_act']['#weight'] + 10 );
  }

  $render_array['nomenclatural_act']['footnotes'] = markup_to_render_array(render_footnotes(),100);

  // END of nomenclatural act block
  RenderHints::setFootnoteListKey($last_footnote_listkey );

  if($typified_name){
    $render_array['typified_name'] = markup_to_render_array('<p class="typified-name">for ' . render_taxon_or_name($typified_name, url(path_to_name($typified_name->uuid))) . '</p>', 40);
  }

  // registration date and office
  $registration_date_insitute_markup = render_registration_date_and_institute($registration_dto);
  if($registration_date_insitute_markup){
    $render_array['registration_date_and_institute'] = markup_to_render_array(
      $registration_date_insitute_markup . '</p>',
      100);
  }

  $render_array['page_footnotes'] = markup_to_render_array(render_footnotes(), 110);

  return $render_array;
}


/**
 * Composes a compact representation for a registrationDTO object
 *
 * Registrations which are not yet published are suppressed.
 *
 * @param $registration_dto
 * @param $style string
 *   The style of how to compose the 'identifier' and 'registration_date_and_institute' part with the summary
 *   - 'citation': Similar to the arrearance of nomenclatural acts in print media
 *   - 'list-item' : style suitable for result lists etc
 *
 * @return array
 *    A drupal render array with the elements:
 *    - 'registration-metadata' when $style == 'list-item'
 *    - 'summary'
 * @ingroup compose
 */
function compose_registration_dto_compact($registration_dto, $style = 'citation', $tag_enclosing_summary = 'p')
{
  $render_array = array();
  $media_link_map = array();

  if(!(isset($registration_dto->identifier) && $registration_dto->status == 'PUBLISHED')){
    return $render_array;
  }

  $registration_date_insitute_markup = render_registration_date_and_institute($registration_dto, 'span');
  $itentifier_markup = l($registration_dto->identifier, path_to_registration($registration_dto->identifier), array('attributes' => array('class' => array('identifier'))));

  $tagged_text_options = array();
  if(isset($registration_dto->nameRef)){
    $tagged_text_options[] = array(
      'filter-type' => 'name',
      'prefix' => '<span class="registered_name">',
      'suffix' => '</span>',
    );
  } else {
    $tagged_text_options[] = array(
      'filter-type' => 'name',
      'prefix' => '<span class="referenced_typified_name">',
      'suffix' => '</span>',
    );
  }
  cdm_tagged_text_add_options($registration_dto->summaryTaggedText, $tagged_text_options);
  $taggged_text_expanded = cdm_tagged_text_expand_entity_references($registration_dto->summaryTaggedText);
  foreach ($taggged_text_expanded  as $tagged_text){
    if(isset($tagged_text->entityReference->type) && $tagged_text->entityReference->type == 'SpecimenTypeDesignation') {
      $mediaDTOs = cdm_ws_get('typedesignation/$0/media', array($tagged_text->entityReference->uuid));
      if(isset($mediaDTOs[0]->uri)){
        $media_url_key = '{link-' . $mediaDTOs[0]->uuid . '}';
        $tagged_text->text = str_replace('[icon]', '[icon]' . $media_url_key, $tagged_text->text);
        $media_link_map[$media_url_key] =  cdm_external_uri($mediaDTOs[0]->uri, true);
      }
    }
  }
  $registation_markup = cdm_tagged_text_to_markup($taggged_text_expanded);
  foreach($media_link_map as $media_url_key => $link){
    $registation_markup = str_replace($media_url_key, $link, $registation_markup);
  }
  if($style == 'citation') {
    $registation_markup = $registation_markup . ' ' . $itentifier_markup . ' ' . $registration_date_insitute_markup;
  } else {
    $render_array['registration-metadata'] = markup_to_render_array('<div class="registration-metadata">' . $itentifier_markup . ' ' . $registration_date_insitute_markup. "</div>", -10);
  }
  $render_array['summary'] = markup_to_render_array('<' . $tag_enclosing_summary . ' class="registration-summary">' . $registation_markup . '</' . $tag_enclosing_summary . '>', 0);

  return $render_array;
}


/**
 * Renders the registrationDate and institutionTitleCache of the $registration_dto as markup.
 *
 * @param $registration_dto
 * @return string
 *    The markup or an empty string
 */
function render_registration_date_and_institute($registration_dto, $enclosing_tag = 'p') {
  $registration_date_institute_markup = '';
  if ($registration_dto->registrationDate) {
    $date_string = format_datetime($registration_dto->registrationDate);
    if (isset($registration_dto->institutionTitleCache) && $registration_dto->institutionTitleCache) {
      $registration_date_institute_markup =
        t("Registration on @date in @institution", array(
          '@date' => $date_string,
          '@institution' => $registration_dto->institutionTitleCache,
        ));
    } else {
      $registration_date_institute_markup =
        t("Registration on @date", array(
          '@date' => $date_string
        ));
    }
    $registration_date_institute_markup = '<' .$enclosing_tag . ' class="registration-date-and-institute">'. $registration_date_institute_markup . '</' .$enclosing_tag . '>';
  }
  return $registration_date_institute_markup;
}


/**
 * @param $registrations
 * @return string
 */
function render_registrations($registrations)
{
  $registration_markup = '';
  $registration_markup_array = array();
  if ($registrations) {
    foreach ($registrations as $reg) {
      $registration_markup_array[] = render_registration($reg);
    }
    $registration_markup = " Registration" . (count($registration_markup_array) > 1 ? 's: ' : ': ')
      . join(', ', $registration_markup_array);
  }
  return $registration_markup;
}


/**
 * Renders a registration
 *
 * TODO replace by compose_registration_dto_compact
 * @param $registration
 */
function render_registration($registration){
  $markup = '';

  if(isset($registration->identifier) && $registration->status == 'PUBLISHED'){
    $office_class_attribute = '';
    if(isset($registration->institution->titleCache)){
      $office_class_attribute = registration_intitute_class_attribute($registration);
    }
    $markup = "<span class=\"registration $office_class_attribute\">" . l($registration->identifier, path_to_registration($registration->identifier)) . ', '
      .  preg_replace('/^([^T]*)(.*)$/', '${1}', $registration->registrationDate)
      . '</span>';
  }
  return $markup;
}

/**
 * @param $registration
 * @return string
 */
function registration_intitute_class_attribute($registration_dto)
{
  if(isset($registration_dto->institutionTitleCache)){
    $institutionTitleCache = $registration_dto->institutionTitleCache;
  } else {
    // fall back option to also support cdm entities
    $institutionTitleCache = @$registration_dto->institution->titleCache;
  }
  return $institutionTitleCache ? 'registration-' . strtolower(preg_replace('/[^a-zA-Z0-9]/', '-', $institutionTitleCache)) : '';
}


/**
 * Renders and array of CDM TypeDesignations
 *
 *  - NameTypeDesignation
 *  - SpecimenTypeDesignation
 *  - TextualTypeDesignation
 *
 * @param object $type_designations an array of cdm TypeDesignation entities
 *  to render
 * @param string $enclosing_tag the tag element type to enclose the whole list
 *  of type designation with. By default this DOM element is <ul>
 * @param string $element_tag the tag element type to be used for each
 *  type designation item.
 * @param bool $link_to_specimen_page whether a specimen in type designation element
 *  should be a link or not.
 *
 * @return string The markup.
 *
 * @InGroup Render
 */
function render_type_designations($type_designations, $enclosing_tag = 'ul', $element_tag =  'li', $link_to_specimen_page = true) {

  // need to add element to render path since type designations
  // need other name render template
  RenderHints::pushToRenderStack('typedesignations');

  $out = '<' . $enclosing_tag .' class="typeDesignations">';
  $specimen_type_designations = array();
  $name_type_designations = array();
  $textual_type_designations = array();
  $separator = ',';

  foreach ($type_designations as $type_designation) {
    switch ($type_designation->class) {
      case 'SpecimenTypeDesignation':
        $specimen_type_designations[] = $type_designation;
        break;
      case 'NameTypeDesignation':
        $name_type_designations[] = $type_designation;
        break;
      case 'TextualTypeDesignation':
        $textual_type_designations[] = $type_designation;
        break;
      default:  throw new Exception('Unknown type designation class: ' . $type_designation->class);
    }
  }

  // NameTypeDesignation ..................................
  if(!empty($name_type_designations)){
    usort($name_type_designations, "compare_type_designations_by_status");
    foreach($name_type_designations as $name_type_designation){
      if ($name_type_designation->notDesignated) {
        $out .= '<'. $element_tag .' class="' . html_class_attribute_ref($name_type_designation) . '">' .  type_designation_status_label_markup($name_type_designation)  . ': '
          . t('not designated') . '</'. $element_tag .'>';
      }
      elseif (isset($name_type_designation->typeName)) {
        $link_to_name_page = url(path_to_name($name_type_designation->typeName->uuid));
        $out .= '<'. $element_tag .' class="' . html_class_attribute_ref($name_type_designation) . '">' .  type_designation_status_label_markup($name_type_designation) ;

        if (!empty($name_type_designation->source->citation)) {
          $out .= type_designation_citation_layout($name_type_designation, $separator); // TODO type_designation_citation_layout() needs most probably to be replaced

        }
        $referenceUri = '';
        if (isset($name_type_designation->typeName->nomenclaturalSource->citation)) {
          $referenceUri = url(path_to_reference($name_type_designation->typeName->nomenclaturalSource->citation->uuid));
        }
        $out .= ': ' . render_taxon_or_name($name_type_designation->typeName, $link_to_name_page, $referenceUri, TRUE, TRUE);
      }
      RenderHints::setAnnotationsAndSourceConfig(annotations_and_sources_config_typedesignations());
      $annotations_and_sources = handle_annotations_and_sources($name_type_designation);
      $out .= $annotations_and_sources['foot_note_keys'];
    }
  } // END NameTypeDesignation

  // SpecimenTypeDesignation ...................................
  if (!empty($specimen_type_designations)) {
    usort($specimen_type_designations, "compare_specimen_type_designation");
    foreach ($specimen_type_designations as $specimen_type_designation) {
      $type_citation_markup = '';

      if (!empty($specimen_type_designation->source->citation)) {

        $citation_footnote_str = cdm_reference_markup($specimen_type_designation->source->citation, null, false, true);
        $author_team = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $specimen_type_designation->source->citation->uuid);

        if (!empty($author_team->titleCache)) {
          $year = @timePeriodToString($specimen_type_designation->source->citation->datePublished, true, 'YYYY');
          $authorteam_str = $author_team->titleCache . ($year ? ' ' : '') . $year;
          if ($authorteam_str == $specimen_type_designation->source->citation->titleCache) {
            $citation_footnote_str = '';
          }
        } else {
          $authorteam_str = $citation_footnote_str;
          // no need for a footnote in case in case it is used as replacement for missing author teams
          $citation_footnote_str = '';
        }

        // for being registered a typedesignation MUST HAVE a citation, so it is save to handle the
        // Registration output in if condition checking if the citation is present
        $registration_markup = render_registrations($specimen_type_designation->registrations);
        $citation_footnote_str .= ($citation_footnote_str ? ' ' : '') . $registration_markup;

        $footnote_key_markup = '';
        if ($citation_footnote_str) {
          // footnotes should be rendered in the parent element so we
          // are relying on the FootnoteListKey set there
          $_fkey2 = FootnoteManager::addNewFootnote(RenderHints::getFootnoteListKey(), $citation_footnote_str);
          $footnote_key_markup = render_footnote_key($_fkey2, $separator, TRUE);
        }

        $type_citation_markup .= '&nbsp;(' . t('designated by') . '&nbsp;<span class="typeReference">' . $authorteam_str . '</span>';
        if (!empty($specimen_type_designation->source->citationMicroReference)) {
          $type_citation_markup .= ': ' . trim($specimen_type_designation->source->citationMicroReference);
        }
        $type_citation_markup .= $footnote_key_markup . ')';

      }


      $out .= '<'. $element_tag .' class="' . html_class_attribute_ref($specimen_type_designation) . '">';
      $out .= type_designation_status_label_markup($specimen_type_designation) . $type_citation_markup;


      $derivedUnitFacadeInstance = null;
      if (isset($specimen_type_designation->typeSpecimen)) {
        $derivedUnitFacadeInstance = cdm_ws_get(CDM_WS_DERIVEDUNIT_FACADE, $specimen_type_designation->typeSpecimen->uuid);
      }

      if (!empty($derivedUnitFacadeInstance->titleCache)) {
        $specimen_markup = $derivedUnitFacadeInstance->titleCache;
        if($link_to_specimen_page && isset($derivedUnitFacadeInstance->specimenLabel) && $derivedUnitFacadeInstance->specimenLabel){
          $specimen_markup = str_replace($derivedUnitFacadeInstance->specimenLabel, l($derivedUnitFacadeInstance->specimenLabel, path_to_specimen($specimen_type_designation->typeSpecimen->uuid)), $specimen_markup);
        }
        RenderHints::setAnnotationsAndSourceConfig(annotations_and_sources_config_typedesignations());
        $annotations_and_sources = handle_annotations_and_sources($derivedUnitFacadeInstance);
        $out .= ': <span class="' . html_class_attribute_ref($specimen_type_designation->typeSpecimen) . '">'
          . $specimen_markup
          . '</span>'; // . ': ' . theme('cdm_specimen', array('specimenTypeDesignation' => $derivedUnitFacadeInstance));
        if(!empty($derivedUnitFacadeInstance->preferredStableUri)){
          $out .= ' ' . l($derivedUnitFacadeInstance->preferredStableUri, $derivedUnitFacadeInstance->preferredStableUri, array('absolute' => true));
        }
        $out .= $annotations_and_sources['foot_note_keys'];
      }
      $out .= '</'. $element_tag .'>';
    }
  } // END Specimen type designations

  // TextualTypeDesignation .........................
  usort($textual_type_designations, 'compare_textual_type_designation');
  if(!empty($textual_type_designations)) {
      RenderHints::setAnnotationsAndSourceConfig([
          // these settings differ from those provided by annotations_and_sources_config_typedesignations()
          // TODO is this by purpose? please document the reason for the difference
          'sources_as_content' => false, // as footnotes
          'link_to_name_used_in_source' => false,
          'link_to_reference' => true,
          'add_footnote_keys' => true,
          'bibliography_aware' => false
        ]
      );
    foreach ($textual_type_designations as $textual_type_designation) {
      $annotations_and_sources = handle_annotations_and_sources($textual_type_designation);
      $encasement =  $textual_type_designation->verbatim ? '"' : '';
      $out .= '<' . $element_tag . ' class="' . html_class_attribute_ref($textual_type_designation) . '">' . type_designation_status_label_markup(null)
        . ': ' .  $encasement . trim($textual_type_designation->text_L10n->text) . $encasement .  $annotations_and_sources['foot_note_keys'] .'</' . $element_tag . '>';
//      if(is_array( $annotations_and_sources['source_references'])){
//        $citation_markup = join(', ', $annotations_and_sources['source_references']);
//      }
//      $out .= $citation_markup;
    }
  }

  // Footnotes for citations, collection acronyms.
  // footnotes should be rendered in the parent element so we
  // are relying on the FootnoteListKey set there
  $_fkey = FootnoteManager::addNewFootnote(
    RenderHints::getFootnoteListKey(),
    (isset($derivedUnitFacadeInstance->collection->titleCache) ? $derivedUnitFacadeInstance->collection->titleCache : FALSE)
  );
  $out .= render_footnote_key($_fkey, $separator);
  $out .= '</' . $enclosing_tag .'>';

  RenderHints::popFromRenderStack();

  return $out;
}


/**
 * Composes the textual representation for the type designation of taxon name identified by the uuid in with a map for the location data.
 *
 * @param $taxon_name_uuid
 * @param $show_specimen_details
 * @return array
 *    A drupal render array with the following elements:
 *    - 'type_designations'
 *    - 'map'
 *    - 'specimens'
 *
 * @ingroup compose
 */
function compose_type_designations($taxon_name_uuid, $show_specimen_details = false)
{
  $render_array = array(
    'type_designations' => array(),
    'map' => array(),
    );
  $type_designations = cdm_ws_get(CDM_WS_PORTAL_NAME_TYPEDESIGNATIONS, $taxon_name_uuid);
  if ($type_designations) {
    usort($type_designations, 'compare_specimen_type_designation');
    $render_array['type_designations'] = markup_to_render_array(
      render_type_designations($type_designations, 'div', 'div')
    );

    $render_array['map'] = compose_type_designations_map($type_designations);
  }
  return $render_array;
}


/**
 * Composes the TypedEntityReference to name type designations passed as associatve array.
 *
 * @param $type_entity_refs_by_status array
 *   an associative array of name type type => TypedEntityReference for name type designations as
 *   produced by the eu.etaxonomy.cdm.api.service.name.TypeDesignationSetManager
 *
 * @ingroup compose
 */
function compose_name_type_designations($type_entity_refs_by_status){
  $render_array = array();
  $preferredStableUri = '';
  foreach($type_entity_refs_by_status as $type_status => $name_type_entityRefs){
    foreach ($name_type_entityRefs as $name_type_entity_ref){
      $type_designation = cdm_ws_get(CDM_WS_TYPEDESIGNATION, array($name_type_entity_ref->uuid, 'preferredUri'));
      $footnote_keys = '';

      if(isset($type_designation->typeSpecimen->preferredStableUri) && $type_designation->typeSpecimen->preferredStableUri){
        $preferredStableUri = $type_designation->typeSpecimen->preferredStableUri;
      }
      // annotations and sources for the $derived_unit_facade_dto
      RenderHints::setAnnotationsAndSourceConfig(annotations_and_sources_config_typedesignations());
      $annotations_and_sources = handle_annotations_and_sources($name_type_entity_ref);

      $render_array[] = markup_to_render_array('<div class="name_type_designation ' . html_class_attribute_ref($name_type_entity_ref)  . '"><span class="type-status">'. ucfirst($type_status) . "</span>: "
        . $name_type_entity_ref->label
        . ($preferredStableUri ? " ". l($preferredStableUri,  $preferredStableUri) : '')
        . $annotations_and_sources['foot_note_keys']
        . '</div>');
      }
  }
  return $render_array;
}

/**
 * Composes the specimen type designations with map from the the $type_entity_refs
 *
 * @param $type_entity_refs array
 *   an associative array of specimen type type => TypedEntityReference for specimen type designations as
 *   produced by the eu.etaxonomy.cdm.api.service.name.TypeDesignationSetManager
 *
 * @param $show_media_specimen
 * @return array
 *    A drupal render array with the following elements:
 *    - 'type_designations'
 *    - 'map'
 *
 * @ingroup compose
 *
 */
function compose_specimen_type_designations($type_entity_refs, $show_media_specimen){

  $render_array = array();

  $type_designation_list = array();
  uksort($type_entity_refs, "compare_type_designation_status_labels");
  foreach($type_entity_refs as $type_status => $type_designation_entity_refs){
    foreach($type_designation_entity_refs as $type_designation_entity_ref){

      $type_designation = cdm_ws_get(CDM_WS_PORTAL_TYPEDESIGNATION, array($type_designation_entity_ref->uuid));
      $type_designation_list[] = $type_designation; // collect for the map

      $derived_unit_facade_dto = cdm_ws_get(CDM_WS_PORTAL_DERIVEDUNIT_FACADE, $type_designation->typeSpecimen->uuid);
      // the media specimen is not contained in the $type_designation returned by CDM_PORTAL_TYPEDESIGNATION, so we need to fetch it separately
      $mediaSpecimen = cdm_ws_get(CDM_WS_PORTAL_OCCURRENCE, array($type_designation->typeSpecimen->uuid, 'mediaSpecimen'));


      $preferredStableUri = '';
      $citation_markup = '';
      $media = '';

      // annotations and sources for the $derived_unit_facade_dto
      RenderHints::setAnnotationsAndSourceConfig(annotations_and_sources_config_typedesignations());
      $annotations_and_sources = handle_annotations_and_sources($derived_unit_facade_dto);
      $source_citations = $annotations_and_sources['source_references'];
      $foot_note_keys = $annotations_and_sources['foot_note_keys'];

      // preferredStableUri
      if(isset($type_designation->typeSpecimen->preferredStableUri) && $type_designation->typeSpecimen->preferredStableUri){
        $preferredStableUri = $type_designation->typeSpecimen->preferredStableUri;
      }

      if($show_media_specimen && $mediaSpecimen){
        // compose output
        // mediaURI
        if(isset($mediaSpecimen->representations[0])) {
          $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME);
          $captionElements = array(
            '#uri' => t('open media'),
            'elements' => array('-none-'),
            'sources_as_content' => true
          );
          $media = compose_cdm_media_gallerie(array(
            'mediaList' => array($mediaSpecimen),
            'galleryName' => CDM_DATAPORTAL_TYPE_SPECIMEN_GALLERY_NAME . '_' . $type_designation_entity_ref->uuid,
            'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
            'cols' => $gallery_settings['cdm_dataportal_media_cols'],
            'captionElements' => $captionElements,
          ));
        }
        // citation and detail for the media specimen
        RenderHints::setAnnotationsAndSourceConfig(annotations_and_sources_config_typedesignations());
        $annotations_and_sources = handle_annotations_and_sources($mediaSpecimen);
        if(is_array( $annotations_and_sources['source_references'])){
          $source_citations = array_merge($source_citations, $annotations_and_sources['source_references']);
        }
        if($annotations_and_sources['foot_note_keys']){
          $foot_note_keys .= ', ' . $annotations_and_sources['foot_note_keys'];
        }
      }

      $citation_markup = join(', ', $source_citations);

      $specimen_markup = $derived_unit_facade_dto->titleCache;
      if(isset($derived_unit_facade_dto->specimenLabel) && $derived_unit_facade_dto->specimenLabel){
        $specimen_markup = str_replace(
          $derived_unit_facade_dto->specimenLabel,
          l($derived_unit_facade_dto->specimenLabel, path_to_specimen($type_designation->typeSpecimen->uuid)), $specimen_markup);
      }

      $type_designation_render_array = markup_to_render_array(
        '<div class="type_designation_entity_ref ' . html_class_attribute_ref($type_designation_entity_ref)  . '">
          <span class="type-status">' . ucfirst($type_status) . "</span>: "
        . $specimen_markup . $foot_note_keys
        . ($citation_markup ? ' '. $citation_markup : '')
        . ($preferredStableUri ? " ". l($preferredStableUri,  $preferredStableUri) : '')
        . $media
        . '</div>');

      $render_array['type_designations'][] = $type_designation_render_array;
    }
  }
  if(count($type_designation_list) > 0 ){
    $render_array['map'] = compose_type_designations_map($type_designation_list);
  } else {
    $render_array['map'] = array();
  }
  return $render_array;
}

/**
 * Creates the markup for the given name relationship
 *
 * @param $name_rel the CDM NameRelationship entity
 * @param $current_name_uuid
 * @param $current_taxon_uuid
 * @param $suppress_if_current_name_is_source // FIXME UNUSED !!!!
 * @param $show_name_cache_only
 *    The nameCache will be shown instead of the titleCache if this parameter is true.
 * @return null|string
 *    The markup or null
 */
function name_relationship_markup($name_rel, $current_name_uuid, $current_taxon_uuid, $show_name_cache_only = false){

  $relationship_markup = null;

  $current_name_is_toName = $current_name_uuid == $name_rel->toName->uuid;

  if($current_name_is_toName){
    $name = $name_rel->fromName;
  } else {
    $name = $name_rel->toName;
  }

  cdm_load_tagged_full_title($name);

  $highlited_synonym_uuid = isset ($name->taxonBases[0]->uuid) ? $name->taxonBases[0]->uuid : '';
  if(!$show_name_cache_only){
    $relationship_markup = render_taxon_or_name($name,
      url(path_to_name($name->uuid, $current_taxon_uuid, $highlited_synonym_uuid, false))
    );
  } else {
    $relationship_markup = l(
      '<span class="' . html_class_attribute_ref($name) . '"">' . $name->nameCache . '</span>',
      path_to_name($name->uuid, $current_taxon_uuid, $highlited_synonym_uuid, false),
      array('html' => true)
    );
  }

  return $relationship_markup;
}


/**
 * Composes an inline representation of selected name relationships
 *
 * The output of this function will be usually appended to taxon name representations.
 * Only the following types are displayed: LATER_HOMONYM, TREATED_AS_LATER_HOMONYM, BLOCKING_NAME_FOR, ORTHOGRAPHIC_VARIANT
 *
 * LATER_HOMONYM, TREATED_AS_LATER_HOMONYM, BLOCKING_NAME_FOR are displayed as
 * non {titleCache} nec {titleCache} nec {titleCache} whereas the related names
 * are ordered alphabetically.
 *
 * ORTHOGRAPHIC_VARIANT is displayed as 'ort. var. {nameCache}'
 *
 * Related issues:
 *   - https://dev.e-taxonomy.eu/redmine/issues/5697 "Show name conserved against as [non xxx]"
 *   - https://dev.e-taxonomy.eu/redmine/issues/6678 "How to correctly show name relationship "orth. var." in dataportal"
 *   - https://dev.e-taxonomy.eu/redmine/issues/5857
 *   - https://dev.e-taxonomy.eu/redmine/issues/2001 "[Cichorieae Portal] Name Relationship -> blocking name are not shown"
 *
 * @param $name_relations
 *    The list of CDM NameRelationsips
 * @param $current_name_uuid
 *    The Uuid of the name for which the relations are to be rendered, the current name will be hidden when
 *    rendering the relation an only the other name is shown. Parameter is REQUIRED.
 * @param $suppress_if_current_name_is_source
 *    The display of the relation will be
 *    suppressed is the current name is on the source of the relation edge.
 *    That is if it is on the from side of the relation. Except for 'blocking name for' which is
 *    an inverse relation. For this relation type the toName is taken in to account.
 * @param $current_taxon_uuid
 *    The taxon to be omitted from related taxa. This is only used to create links, see path_to_name()
 * @return array
 *    A drupal render array
 *
 * @ingroup Compose
 */
function compose_name_relationships_inline($name_relations, $current_name_uuid, $current_taxon_uuid, $suppress_if_current_name_is_source = true) {

  RenderHints::pushToRenderStack('homonym');
  // the render stack element homonyms is being used in the default render templates !!!, see CDM_NAME_RENDER_TEMPLATES_DEFAULT

  $selected_name_rel_uuids = variable_get(CDM_NAME_RELATIONSHIP_INLINE_TYPES, unserialize(CDM_NAME_RELATIONSHIP_INLINE_TYPES_DEFAULT));
  $name_rel_type_filter = array('direct' => array(), 'inverse' => array());
  foreach ($selected_name_rel_uuids as $uuid){
    $name_rel_type_filter['direct'][$uuid] = $uuid;
    if($uuid != UUID_NAMERELATIONSHIPTYPE_MISSPELLING){
      $name_rel_type_filter['inverse'][$uuid] = $uuid;
    }
  }

  $list_prefix = '<span class="name_relationships">[';
  $list_suffix = ']</span>';
  $item_prefix = '<span class="item">';
  $item_suffix = '</span> ';
  $render_array = compose_name_relationships($name_relations, $name_rel_type_filter, $current_name_uuid, $current_taxon_uuid, $list_prefix, $list_suffix, $item_prefix, $item_suffix);

  // remove the glue space from the last item element which has been added by the $item_suffix = '</span> '
  $items_ctn = count($render_array['list']['items']);
  if($items_ctn){
    $render_array['list']['items'][$items_ctn - 1]['#suffix'] = '</span>';
  }

  RenderHints::popFromRenderStack();
  return $render_array;
}

/**
 * Composes an list representation of the name relationships.
 *
 * The output of this function will be usually appended to taxon name representations.
 *
 * Related issues:
 *   - https://dev.e-taxonomy.eu/redmine/issues/5697 "Show name conserved against as [non xxx]"
 *   - https://dev.e-taxonomy.eu/redmine/issues/6678 "How to correctly show name relationship "orth. var." in dataportal"
 *   - https://dev.e-taxonomy.eu/redmine/issues/5857
 *
 * @param $name_relations
 *    The list of CDM NameRelationsips
 * @param $current_name_uuid
 *    The Uuid of the name for which the relations are to be rendered, the current name will be hidden when
 *    rendering the relation an only the other name is shown. Parameter is REQUIRED.
 * @param $current_taxon_uuid
 *    The taxon to be omitted from related taxa. This is only used to create links, see path_to_name()
 * @return array
 *    A drupal render array
 *
 * @ingroup Compose
 */
function compose_name_relationships_list($name_relations, $current_name_uuid, $current_taxon_uuid) {

  // $ordered_name_relation_type_uuids = array_keys(cdm_terms_by_type_as_option('NameRelationshipType', CDM_ORDER_BY_ORDER_INDEX_ASC));

  $key = 'name_relationships';
  RenderHints::pushToRenderStack($key);
  if(RenderHints::isUnsetFootnoteListKey()){
    RenderHints::setFootnoteListKey($key);
  }
  // the render stack element homonyms is being used in the default render templates !!!, see CDM_NAME_RENDER_TEMPLATES_DEFAULT

  $selected_name_rel_uuids = variable_get(CDM_NAME_RELATIONSHIP_LIST_TYPES, cdm_vocabulary_as_defaults(UUID_NAME_RELATIONSHIP_TYPE));
  $name_rel_type_filter = array('direct' => array(), 'inverse' => array());
  foreach ($selected_name_rel_uuids as $uuid){
    $name_rel_type_filter['direct'][$uuid] = $uuid;
    $name_rel_type_filter['inverse'][$uuid] = $uuid;
  }

  $list_prefix = '<div class="relationships_list name_relationships">';
  $list_suffix = '</div>';
  $item_prefix = '<div class="item">';
  $item_suffix = '</div>';

  $render_array = compose_name_relationships($name_relations, $name_rel_type_filter, $current_name_uuid, $current_taxon_uuid, $list_prefix, $list_suffix, $item_prefix, $item_suffix);

  RenderHints::popFromRenderStack();
  if(RenderHints::getFootnoteListKey() == $key) {
    $render_array['footnotes'] = markup_to_render_array(render_footnotes(RenderHints::getFootnoteListKey()));
    RenderHints::clearFootnoteListKey();
  }
  return $render_array;
}

/**
 * @param $name_relations
 * @param $name_rel_type_filter
 *   Associative array with two keys:
 *   - 'direct': the relationship type uuids for the direct direction of the relation edge to be included
 *   - 'inverse': the relationship type uuids for the direct direction of the relation edge to be included
 * @param $current_name_uuid
 * @param $current_taxon_uuid
 * @param $list_prefix
 * @param $list_suffix
 * @param $item_prefix
 * @param $item_suffix
 * @return array
 *
 * @ingroup Compose
 */
function compose_name_relationships($name_relations, $name_rel_type_filter, $current_name_uuid, $current_taxon_uuid,
                                    $list_prefix, $list_suffix, $item_prefix, $item_suffix)
{
  $non_nec_name_reltype_uuids = array(UUID_NAMERELATIONSHIPTYPE_LATER_HOMONYM,
    UUID_NAMERELATIONSHIPTYPE_TREATED_AS_LATER_HOMONYM,
    UUID_NAMERELATIONSHIPTYPE_CONSERVED_AGAINST,
    UUID_NAMERELATIONSHIPTYPE_MISSPELLING,
    UUID_NAMERELATIONSHIPTYPE_BLOCKING_NAME_FOR);

  $render_array = array(
    'list' => array(
      '#prefix' => $list_prefix,
      '#suffix' => $list_suffix,
      'items' => array()
    ),
    'footnotes' => array()
  );

  if ($name_relations) {

    // remove all relations which are not selected in the settings and
    // separate all LATER_HOMONYM, TREATED_AS_LATER_HOMONYM, BLOCKING_NAME_FOR relations and ORTHOGRAPHIC_VARIANTs
    // for special handling
    $filtered_name_rels = array();
    $non_nec_name_rels = array();
    $orthographic_variants = array();
    foreach ($name_relations as $name_rel) {
      $rel_type_uuid = $name_rel->type->uuid;
      $is_inverse_relation = $current_name_uuid == $name_rel->toName->uuid;
      if ((!$is_inverse_relation && isset($name_rel_type_filter['direct'][$rel_type_uuid]) && $name_rel_type_filter['direct'][$rel_type_uuid])
        ||($is_inverse_relation && isset($name_rel_type_filter['inverse'][$rel_type_uuid]) && $name_rel_type_filter['inverse'][$rel_type_uuid])) {

        if (array_search($rel_type_uuid, $non_nec_name_reltype_uuids) !== false && (
            $current_name_uuid == $name_rel->fromName->uuid && $rel_type_uuid != UUID_NAMERELATIONSHIPTYPE_BLOCKING_NAME_FOR
            || $current_name_uuid == $name_rel->toName->uuid && $rel_type_uuid == UUID_NAMERELATIONSHIPTYPE_BLOCKING_NAME_FOR
          )
        ){
          $non_nec_name_rels[] = $name_rel;
        } else if (UUID_NAMERELATIONSHIPTYPE_ORTHOGRAPHIC_VARIANT == $rel_type_uuid) {
          $orthographic_variants[] = $name_rel;
        } else {

          $filtered_name_rels[] = $name_rel;
        }
      }
    }
    $name_relations = $filtered_name_rels;

    usort($name_relations, 'compare_name_relations_by_term_order_index');

    // compose
    foreach ($name_relations as $name_rel) {

      $is_inverse_relation = $current_name_uuid == $name_rel->toName->uuid;

      $rel_footnote_key_markup = handle_name_relationship_as_footnote($name_rel);
      $relationship_markup = name_relationship_markup($name_rel, $current_name_uuid, $current_taxon_uuid);

      $label = cdm_relationship_type_term_abbreviated_label($name_rel->type, $is_inverse_relation);
      $symbol = cdm_relationship_type_term_symbol($name_rel->type, $is_inverse_relation);
      $symbol_markup = '<span class="symbol" title="' . $label . '">' . $symbol . '</span>' . $rel_footnote_key_markup . ' ';
      $relationship_markup = $symbol_markup . $relationship_markup;
      if ($relationship_markup) {
        $render_array['list']['items'][] = markup_to_render_array($relationship_markup,
          null,
          $item_prefix,
          $item_suffix);
      }
    }

    // name relationships to be displayed as non nec
    if (count($non_nec_name_rels) > 0) {
      $non_nec_markup = '';
      foreach ($non_nec_name_rels as $name_rel) {
        $is_inverse_relation = $current_name_uuid == $name_rel->toName->uuid;
        $rel_footnote_key_markup = handle_name_relationship_as_footnote($name_rel);
        $relationship_markup = name_relationship_markup($name_rel, $current_name_uuid, $current_taxon_uuid);
        $label = cdm_relationship_type_term_abbreviated_label($name_rel->type, $is_inverse_relation);
        $symbol = $non_nec_markup ? ' nec ' : 'non';
        $symbol_markup = '<span class="symbol" title="' . $label . '">' . $symbol . '</span>' . $rel_footnote_key_markup .  ' ';
        $non_nec_markup .= $symbol_markup . $relationship_markup;
      }
      if ($non_nec_markup) {
        $render_array['list']['items'][] = markup_to_render_array($non_nec_markup,
          null,
          $item_prefix,
          $item_suffix);
      }
    }

    // orthographic variants
    if (count($orthographic_variants) > 0) {
      foreach ($orthographic_variants as $name_rel) {

        $is_inverse_relation = $current_name_uuid == $name_rel->toName->uuid;
        $rel_footnote_key_markup = handle_name_relationship_as_footnote($name_rel);
        $relationship_markup = name_relationship_markup($name_rel, $current_name_uuid, $current_taxon_uuid, TRUE);
        $nomref_footnote_key_markup = handle_nomenclatural_reference_as_footnote($name_rel->toName);
        $label = cdm_relationship_type_term_abbreviated_label($name_rel->type, $is_inverse_relation);
        $symbol = cdm_relationship_type_term_symbol($name_rel->type, $is_inverse_relation);
        $symbol_markup = '<span class="symbol" title="' . $label . '">' . $symbol . '</span>' . $rel_footnote_key_markup .  ' ';
        $relationship_markup = $symbol_markup . $relationship_markup . $nomref_footnote_key_markup;
      }
      if (isset($relationship_markup) && $relationship_markup) {
        $render_array['list']['items'][] = markup_to_render_array($relationship_markup,
          null,
          $item_prefix,
          $item_suffix);
      }
    }
  }
  return $render_array;
}



/**
 * @param $taxon
 * @return array
 */
function cdm_name_relationships_for_taxon($taxon)
{
  $from_name_relations = cdm_ws_get(CDM_WS_PORTAL_TAXON_FROM_NAMERELATIONS, $taxon->uuid);
  $to_name_relations = cdm_ws_get(CDM_WS_PORTAL_TAXON_TO_NAMERELATIONS, $taxon->uuid);
  $name_relations = array_merge($from_name_relations, $to_name_relations);
  return $name_relations;
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

function nameTypeToDTYPE($dtype){
  static $nameTypeLabelMap = array(
    "ICNB" => "BacterialName",
    "ICNAFP" => "BotanicalName",
    "ICNCP" => "CultivarPlantName",
    "ICZN" => "ZoologicalName",
    "ICVCN" => "ViralName",
    "Any taxon name" => "TaxonName",
    "NonViral" => "TaxonName",
    "Fungus" => "BotanicalName",
    "Plant" => "BotanicalName",
    "Algae" => "BotanicalName",
  );
  return $nameTypeLabelMap[$dtype];

}


function compare_name_relations_by_term_order_index($name_rel1, $name_rel2){
  return compare_terms_by_order_index($name_rel1->type, $name_rel2->type);
}

/**
 * Provides an array with the different registration types covered by the passed registration.
 *
 * The labels in the returned array are translatable.
 *
 * See also https://dev.e-taxonomy.eu/redmine/issues/8016
 *
 * @param $registration_dto
 * @return array
 *    An array of the labels describing the different registration types covered by the passed registration.
 */
function registration_types($registration_dto){
  $reg_type_labels = array();
  if(isset($registration_dto->nameRef)){
    $reg_type_labels["name"] = t("new name");
    $reg_type_labels["taxon"] = t("new taxon");
    $name_relations = cdm_ws_fetch_all(str_replace("$0", $registration_dto->nameRef->uuid, CDM_WS_PORTAL_NAME_NAME_RELATIONS));
    $is_new_combination = true;
    foreach($name_relations as $name_rel){
      if(isset($name_rel->type->uuid)){
        $name_is_from_name = $registration_dto->nameRef->uuid == $name_rel->fromName->uuid;
        switch($name_rel->type->uuid) {
          case UUID_NAMERELATIONSHIPTYPE_BASIONYM:
            if(!$name_is_from_name){
              $reg_type_labels["basionym"] = t("new combination");
              $is_new_combination = true;
            }
            break;
          case UUID_NAMERELATIONSHIPTYPE_REPLACED_SYNONYM:
            if(!$name_is_from_name) {
              $is_new_combination = true;
            }
            break;
          case UUID_NAMERELATIONSHIPTYPE_VALIDATED_BY_NAME:
            if(!$name_is_from_name) {
              $reg_type_labels["validation"] = t("validation");
            }
            break;
          case UUID_NAMERELATIONSHIPTYPE_ORTHOGRAPHIC_VARIANT:
            if(!$name_is_from_name) {
              $reg_type_labels["orth_var"] = t("orthographical correction");
            }break;
          default:
            // NOTHING
        }
      }
    }
    if($is_new_combination){
      unset($reg_type_labels["taxon"]);
    }
  }
  if(isset($registration_dto->orderdTypeDesignationWorkingSets)){
    $reg_type_labels[] = t("new nomenclatural type");
  }
  return $reg_type_labels;
}

/**
 * Collects and deduplicates the type designations associated with the passes synonyms.
 *
 * @param $synonymy_group
 *    An array containing a homotypic or heterotypic group of names.
 * @param $accepted_taxon_name_uuid
 *    The uuid of the accepted taxon name. Optional parameter which is required when composing
 *    the information for the homotypic group. In this case the accepted taxon is not included
 *    in the $synonymy_group and must therefor passed in this second parameter.
 *
 * @return array
 *    The type designations
 */
function type_designations_for_synonymy_group($synonymy_group, $accepted_taxon_name_uuid = null)
{
  if (count($synonymy_group) > 0) {
    $name_uuid = array_pop($synonymy_group)->name->uuid;
  } else {
    $name_uuid = $accepted_taxon_name_uuid;
  }
  if ($name_uuid) {
   $type_designations = cdm_ws_get(CDM_WS_PORTAL_NAME_TYPEDESIGNATIONS_IN_HOMOTYPICAL_GROUP, $name_uuid);
    if ($type_designations) {
      return $type_designations;
    }
  }

  return array();
}


/**
 * Compares two SpecimenTypeDesignations
 *
 * @param object $a
 *   A SpecimenTypeDesignation.
 * @param object $b
 *   SpecimenTypeDesignation.
 */
function compare_specimen_type_designation($a, $b) {

  $cmp_by_status = compare_type_designations_by_status($a,$b);
  if($cmp_by_status !== 0){
    return $cmp_by_status;
  }

  $aQuantifier = FALSE;
  $bQuantifier = FALSE;
  if ($aQuantifier == $bQuantifier) {
    // Sort alphabetically.
    $a_text =  isset($a->typeSpecimen->titleCache) ? preg_replace('/[\[\]\"]/', '', $a->typeSpecimen->titleCache) : '';
    $b_text =  isset($b->typeSpecimen->titleCache) ? preg_replace('/[\[\]\"]/', '', $b->typeSpecimen->titleCache) : '';
    return strcasecmp($a_text, $b_text);
  }
  return ($aQuantifier < $bQuantifier) ? -1 : (($aQuantifier > $bQuantifier) ? 1 : 0);
}

/**
 * Compares the status of two TypeDesignations
 *
 * @param object $a
 *   A TypeDesignation
 * @param object $b
 *   TypeDesignation
 */
function compare_type_designations_by_status($a, $b) {
  $status_a = isset($a->typeStatus) ? $a->typeStatus : null;
  $status_b = isset($b->typeStatus) ? $b->typeStatus : null;
  return compare_type_designation_status($status_a, $status_b);
}

/**
 * Compares two TypeDesignationStatusBase
 *
 * @param object $a
 *   A TypeDesignationStatusBase.
 * @param object $b
 *   TypeDesignationStatusBase.
 */
function compare_type_designation_status($a, $b) {
  $type_status_order = type_status_order();
  $aQuantifier = FALSE;
  $bQuantifier = FALSE;
  if (isset($a->label) && isset($b->label)) {
    $aQuantifier = array_search($a->label, $type_status_order);
    $bQuantifier = array_search($b->label, $type_status_order);
  }
  return ($aQuantifier < $bQuantifier) ? -1 : (($aQuantifier > $bQuantifier) ? 1 : 0);
}

/**
 * Compares the two TextualTypeDesignations
 *
 * @param object $a
 *   A TextualTypeDesignations.
 * @param object $b
 *   TextualTypeDesignations.
 */
function compare_textual_type_designation($a, $b) {

  $cmp_by_status = compare_type_designations_by_status($a,$b);
  if($cmp_by_status !== 0){
    return $cmp_by_status;
  }

  $aQuantifier = FALSE;
  $bQuantifier = FALSE;
  if ($aQuantifier == $bQuantifier) {
    // Sort alphabetically.
    $a_text =  isset($a->text_L10n->text) ? $a->text_L10n->text : '';
    $b_text =  isset($b->text_L10n->text) ? $b->text_L10n->text : '';
    return strcasecmp($a_text, $b_text);
  }
  return ($aQuantifier < $bQuantifier) ? -1 : (($aQuantifier > $bQuantifier) ? 1 : 0);
}


/**
 * Compares two SpecimenTypeDesignation status labels
 *
 * @param string $a
 *   A TypeDesignationStatus label.
 * @param string $b
 *   A TypeDesignationStatus label.
 */
function compare_type_designation_status_labels($a, $b) {

  $type_status_order = type_status_order();

  $aQuantifier = FALSE;
  $bQuantifier = FALSE;
  if (isset($a) && isset($b)) {
    $aQuantifier = array_search($a, $type_status_order);
    $bQuantifier = array_search($b, $type_status_order);
  }
  return ($aQuantifier < $bQuantifier) ? -1 : 1;
}

/**
 * @return array
 */
function type_status_order()
{
  /*
    This is the desired sort order as of now: Holotype Isotype Lectotype
    Isolectotype Syntype.
    TODO Basically, what we are trying to do is, we define
    an ordered array of TypeDesignation-states and use the index of this array
    for comparison. This array has to be filled with the cdm- TypeDesignation
    states and the order should be parameterisable inside the dataportal.
    */
  // Make that static for now.
  $type_status_order = array(
    'Epitype',
    'Holotype',
    'Isotype',
    'Lectotype',
    'Isolectotype',
    'Syntype',
    'Paratype'
  );
  return $type_status_order;
}

/**
 * Return HTML for the lectotype citation with the correct layout.
 *
 * This function prints the lectotype citation with the correct layout.
 * Lectotypes are renderized in the synonymy tab of a taxon if they exist.
 *
 * @param mixed $typeDesignation
 *   Object containing the lectotype citation to print.
 *
 * @return string
 *   Valid html string.
 */
function type_designation_citation_layout($typeDesignation, $footnote_separator = ',') {
  $res = '';
  $citation = $typeDesignation->source->citation;
  $pages = $typeDesignation->source->citationMicroReference;
  if(isset($typeDesignation->typeStatus->uuid) && isset($typeDesignation->typeStatus->representation_L10n)) {
    if ( $typeDesignation->typeStatus->uuid == UUID_NTD_ORIGINAL_DESIGNATION || $typeDesignation->typeStatus->uuid == UUID_NTD_MONOTYPY) {
      $res = ' (' . $typeDesignation->typeStatus->representation_L10n . ')';
      return $res;
    }
  }

  if ($citation) {
    // $type = $typeDesignation_citation->type;
    $year = isset($citation->datePublished->start) ? substr($citation->datePublished->start, 0, 4) : '';
    $author = isset($citation->authorship->titleCache) ? $citation->authorship->titleCache : '';
    $res .= ' (designated by ';
    $res .= $author;
    $res .= ($year ? ' ' . $year : '');
    $res .= ($pages ? ': ' . $pages : '');
    // $res .= ')';

    // footnotes should be rendered in the parent element so we
    // are relying on the FootnoteListKey set there
    $fkey_typeDesignation = FootnoteManager::addNewFootnote(RenderHints::getFootnoteListKey(), $citation->titleCache);
    $res .= render_footnote_key($fkey_typeDesignation, $footnote_separator,TRUE) . ')';
  }
  return $res;
}

/**
 * Creates markup for the status of a type designation. In case the status or its representation is missing the label will be set to "Type"
 *
 * @param $type_designation
 * @return string
 */
function type_designation_status_label_markup($type_designation)
{
  return '<span class="type-status">'
    . ((isset($type_designation->typeStatus->representation_L10n)) ? ucfirst($type_designation->typeStatus->representation_L10n) : t('Type')) . '</span>'
    ;
}
