<?php
/**
 * @file
 * Functions for dealing with CDM entities of type SpecimenOrOccurrences
 *
 * @copyright
 *   (C) 2007-2012 EDIT
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
 * @param $preservedSpecimenDTO
 * @param $detail_html
 * @return string
 */
function compose_cdm_specimen_page($specimenUuid)
{
    drupal_set_title("Specimen Details");
    $preservedSpecimenDTO = cdm_ws_get(CDM_WS_PORTAL_OCCURRENCE, array($specimenUuid, 'specimenDerivates'));

    $render_array = array();
    RenderHints::pushToRenderStack('specimen_page');

    $detail_html = compose_cdm_specimen_page_html($preservedSpecimenDTO, true);
    $render_array['specimen_html'] = array(
        '#markup' => $detail_html
    );

    RenderHints::popFromRenderStack();
    return $render_array;
}

/**
 * @param $preservedSpecimenDTO
 * @return string
 */
function compose_cdm_specimen_page_html($preservedSpecimenDTO, $isSpecimenPage = false)
    //FIXME remove this method and use render_array correctly in compressed table
{
    $detail_html = "";
    //link to specimen page
    $specimenPageLink = l($preservedSpecimenDTO->accessionNumber, path_to_specimen($preservedSpecimenDTO->uuid),array('attributes' => array('target'=>'_blank')));
    $detail_html .= ($isSpecimenPage?"<h2>":"<strong>") . $specimenPageLink . ($isSpecimenPage?"</h2>":"</strong><br>");
    if($isSpecimenPage) {
        $detail_html .= "<br>";
        if($preservedSpecimenDTO->citation){
            $detail_html .= "<strong>Citation:</strong> " . $preservedSpecimenDTO->citation . "<br>";
        }
    }
    if($isSpecimenPage){
        // associated taxa
        if($preservedSpecimenDTO->associatedTaxa){
            $detail_html .= "<br><strong>Associated Taxa:</strong><br>";
            foreach($preservedSpecimenDTO->associatedTaxa as $associatedTaxon){
                $detail_html .= $associatedTaxon."<br>";
            }
        }
    }
    // - type information
    $types = "";
    if (isset($preservedSpecimenDTO->types)) {
        foreach ($preservedSpecimenDTO->types as $typeStatus => $typedTaxa) {
            if($isSpecimenPage){
                //typed taxa
                if($preservedSpecimenDTO->types){
                    $detail_html .= "<br><strong>Type Designations:</strong><br>";
                    $detail_html .= "<i>".$typeStatus."</i>: ";
                    foreach($typedTaxa as $typedTaxon){
                        $detail_html .= $typedTaxon." ";
                    }
                    $detail_html .= "<br>";
                }
            }
            else{
                $types .= $typeStatus." ";
            }
        }
    }
    $derivateDataDTO = $preservedSpecimenDTO->derivateDataDTO;
    // - specimen scans
        $specimenScans = createLinks($derivateDataDTO->specimenScans);
    // - molecular data
    $molecularData = "";
    if ($derivateDataDTO->molecularDataList) {
        foreach ($derivateDataDTO->molecularDataList as $molecular) {
            //provider link
            if (isset($molecular->providerLink)) {
                $molecularData .= createHTMLLink($molecular->providerLink);
            }
            //contig link
            if (isset($molecular->contigFiles[0])) {//FIXME check if empty
                $molecularData .= "[";
                if (isset($molecular->contigFiles)) {
                    foreach ($molecular->contigFiles as $contigFile) {
                        if (isset($contigFile->contigLink)) {
                            $molecularData .= createHTMLLink($contigFile->contigLink);

                            //primer links
                            $molecularData .= createHTMLLinks($contigFile->primerLinks);
                            if (isset($contigFile->primerLinks)) {
                                foreach ($contigFile->primerLinks as $primerLink) {
                                }
                            }
                        }
                    }
                } else {
                    $molecularData .= "no contig";
                }
                $molecularData .= "]";
            }
            //FIXME separate with comma (remove trailing comma)
        }
    }
    // - detail images
    $detailImages = createLinks($derivateDataDTO->detailImages);

    if ($types) {
        $detail_html .= $isSpecimenPage?"<br>":"";
        $detail_html .= "<strong>Types:</strong> " . $types . "<br>";
    }
    if ($specimenScans) {
        $detail_html .= $isSpecimenPage?"<br>":"";
        $detail_html .= "<strong>Specimen Scans:</strong> " . $specimenScans . "<br>";
    }
    if ($molecularData) {
        $detail_html .= $isSpecimenPage?"<br>":"";
        $detail_html .= "<strong>Molecular Data:</strong> " . $molecularData . "<br>";
    }
    if ($detailImages) {
        $detail_html .= $isSpecimenPage?"<br>":"";
        $detail_html .= "<strong>Detail Images:</strong> " . $detailImages . "<br>";
    }

    //character data
    if ($preservedSpecimenDTO->characterData) {
        $detail_html .= $isSpecimenPage?"<br>":"";
        $detail_html .= "<strong>Character Data</strong><br>";
        foreach ($preservedSpecimenDTO->characterData as $characterStatePair) {
            $detail_html .= "<i>" . $characterStatePair->first . ": </i>" . $characterStatePair->second;
            $detail_html .= "<br>";
        }
    }
    return $detail_html;
}

/**
 * Compose an render array from a CDM DerivedUnitFacade object.
 *
 * compose_hook() implementation
 *
 * @param object $specimenOrObservation
 *   the CDM instance of type SpecimenOrObservation to compose
 *   the render array for
 * @param array $derivatives
 *   the render array which contains the compositions of the derivatives
 *   of the supplied $specimenOrObservation
 *
 * @return array
 *   the supplied render array $derivatives to which the composition of the supplied
 *   $specimenOrObservation has been added to
 *
 * @ingroup compose
 */
function compose_cdm_specimenOrObservation($specimenOrObservation, &$derivatives = null) {

  $exclude_occurrence_fields = &drupal_static(__FUNCTION__);
  if (!isset($exclude_occurrence_fields)) {
     $exclude_occurrence_fields = array(
        'derivationEvents',
        'extensions', // TODO ignored for now, how to handle extensions?
        'titleCache',
        'protectedTitleCache',
        'derivedUnitMedia',
        'created',
        'publish',
        'updated',
        'class',
        'uuid',
       ''
    );
  }


  // only show uuid it the user is logged in
  if(user_is_logged_in() && ($a_idx = array_search('uuid', $exclude_occurrence_fields)) !== FALSE ) {
    unset($exclude_occurrence_fields[$a_idx]);
  }

  if (!isset($derivatives)) {
    $derivatives = array();
  }

  $descriptions = null;
  $derivedFrom = null;

  if (is_object($specimenOrObservation)) {

    // request again for deeper initialization
    $specimenOrObservation = cdm_ws_get("portal/" . CDM_WS_OCCURRENCE, $specimenOrObservation->uuid);


    $type_label = $specimenOrObservation->class;
    RenderHints::setFootnoteListKey($type_label . '-' . $specimenOrObservation->uuid);

    // collect typeStatus as label
    if (isset($specimenOrObservation->specimenTypeDesignations)) {
      $type_status = array();
      foreach ($specimenOrObservation->specimenTypeDesignations as $typeDesignation) {
        if (isset($typeDesignation->typeStatus->representation_L10n)){
          $type_status[] = $typeDesignation->typeStatus->representation_L10n;
        }
      }
      if (count($type_status) > 0){
        $type_label = implode(', ', $type_status);
      }
    }

    $title = $type_label . ': '. $specimenOrObservation->titleCache;

    $groups = array();
    // --- add initialized fields
    foreach (get_object_vars($specimenOrObservation) as $field => $value) {
      if (!in_array($field, $exclude_occurrence_fields) && ($value && (!is_object($value) || isset($value->class)))) {
        switch ($field) {

          /* ---- java.lang.Object --- */
          case 'class':
            if($value != '' /* FieldUnit' */){
              @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value);
            }
          break;

          case 'markers':
            $dd_elements = array();
            foreach ($value as $marker) {
              $dd_elements[] = compose_cdm_marker($marker);
            }
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $dd_elements);
            break;


          case 'annotations':
            $dd_elements = array();
            foreach ($value as $annotation) {
              // TODO respect annotation type filter settings
              $dd_elements[] = $annotation->text;
            }
            @_description_list_group_add($groups, t('Notes'), $dd_elements);
            break;

          /* ---- SpecimenOrObservationBase --- */
          case 'sex':
          case 'lifeStage':
          case 'kindOfUnit':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value->representation_L10n);
            break;

          case 'definition':
            // TODO
            break;

          case 'specimenTypeDesignations':
            @_description_list_group_add(
              $groups,
              cdm_occurrence_field_name_label($field),
              array(
                '#markup'=>theme('cdm_typedesignations', array('typeDesignations' => $value)),
              )
            );
            break;

          case 'determinations':
            $dd_elements = array();
            $glue = ', ';

            foreach  ($value as $determinationEvent){
              $timeperiod_string = NULL;
              if (isset($determinationEvent->timeperiod)) {
                $timeperiod_string = timePeriodToString($determinationEvent->timeperiod);
              }
              $weight = isset($determinationEvent->preferred) && $determinationEvent->preferred == 1 ? '0' : ($timeperiod_string ? $timeperiod_string : '1');
              // check key exists
              while (array_key_exists($weight, $dd_elements)) {
                $weight .= '0';
              }
                if($determinationEvent->taxonName){
                    $taxon_name = $determinationEvent->taxonName;
                    $name_link = "";
                }
                else if($determinationEvent->taxon){
                    $taxon_name = cdm_ws_get(CDM_WS_TAXON . '/$0/name', $determinationEvent->taxon->uuid);
                    $name_link = path_to_taxon($determinationEvent->taxon->uuid);
                }
              $taxon_html = theme('cdm_taxonName',
                  array(
                      'taxonName' => $taxon_name,
                      'nameLink' => $name_link,
                  )
              );
              $dd_elements[$weight] = $taxon_html;
              if (isset($determinationEvent->modifier)) {
                $dd_elements[$weight] .= cdm_term_representation($determinationEvent->modifier);
              }
              if ($timeperiod_string) {
                $dd_elements[$weight] .= $glue . $timeperiod_string;
              }
              if (isset($determinationEvent->actor->titleCache)) {
                $dd_elements[$weight] .= $glue . $determinationEvent->actor->titleCache;
              }
              if (isset($determinationEvent->description)) {
                $dd_elements[$weight] .= $glue . $determinationEvent->description;
              }
            }
            ksort($dd_elements);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('determinations'), $dd_elements);
            break;

          case 'descriptions':
            $descriptions = $value;
            $occurrence_featureTree = cdm_get_occurrence_featureTree();
            $dd_elements = array();

            foreach ($value as $description) {
              $description = cdm_ws_get(CDM_WS_PORTAL_DESCRIPTION, $description->uuid);
//               if($description->imageGallery == TRUE) {
//                 continue;
//               }
              $elements_by_feature = _mergeFeatureTreeDescriptions($occurrence_featureTree->root->childNodes, $description->elements);
              $description_render_elements = compose_cdm_feature_nodes($elements_by_feature, null);
              $dd_elements[] = $description_render_elements;
            }

            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $dd_elements);
            break;

          case 'sources':
              $dd_elements = array();
              foreach ($value as $identifiable_source) {
                $dd_elements[] = theme('cdm_OriginalSource', array('source' => $identifiable_source));
              }
              @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $dd_elements);
              break;


          /* ---- DerivedUnitBase --- */
          case 'derivedFrom':
            $derivedFrom = $value;
            break;

          case 'collection':
            $sub_dl_groups = array();
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('code'), $value->code, NULL, 1);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('codeStandard'),  $value->codeStandard, NULL, 2);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('institute'), $value->institute, NULL, 3);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('townOrLocation'), $value->townOrLocation, NULL, 4);
            // TODO "superCollection"
            // TODO may have media

            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
                array(
                    array('#markup' => $value->titleCache),
                    array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                )
            );
            break;

            case 'storedUnder':
              @_description_list_group_add($groups, cdm_occurrence_field_name_label('storedUnder'), theme('cdm_taxonName', $value));
              break;


            /* ---- Specimen --- */
            case 'sequences':
              $dd_elements = array();
              foreach ($value as $sequence) {
                $dd_elements[] = compose_cdm_sequence($sequence);
              }
              @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $dd_elements);
            break;

            // TODO preservation
            // TODO exsiccatum


          /* ---- FieldObservation --- */
          case 'gatheringEvent':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('collector'), $value->actor->titleCache);
            @_description_list_group_add($groups, t('Gathering time'), timePeriodToString($value->timeperiod));
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('description'), $value->description);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('locality'), $value->locality->text);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('country'), $value->country->representation_L10n);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('collectingMethod'), $value->collectingMethod);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('absoluteElevation'), $value->absoluteElevation, ' m');
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('absoluteElevationError'), $value->absoluteElevationError, ' m');
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('distanceToGround'), $value->distanceToGround, ' m');
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('distanceToWaterSurface'), $value->distanceToWaterSurface, ' m');
            if (isset($value->collectingAreas)) {
              $area_representations = array();
              foreach($value->collectingAreas as $area) {
                $area_representations[] = $area->representation_L10n;
              }
              @_description_list_group_add($groups, cdm_occurrence_field_name_label('collectingAreas'), implode(', ', $area_representations));
            }
            if (isset($value->exactLocation)) {
              $sub_dl_groups = array();
              @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('errorRadius'), $value->exactLocation->errorRadius, ' m', 1);
              @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('longitude'), round($value->exactLocation->longitude, 7), '°', 2);
              @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('latitude'), round($value->exactLocation->latitude, 7), '°', 3);
              if (isset($value->exactLocation->referenceSystem)) {
                @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('referenceSystem'), $value->exactLocation->referenceSystem->representation_L10n, '', 4);
              }

              @_description_list_group_add($groups, cdm_occurrence_field_name_label('exactLocation'),
                  array(
                      array('#markup' => $value->exactLocation->sexagesimalString),
                      array(
                          '#theme' => 'description_list',
                          '#groups' => $sub_dl_groups
                      ),
                  )
              );
            }
            break;

          default:
            if(is_object($value) || is_array($value)){
              drupal_set_message("Unhandled type in compose_cdm_specimenOrObservation() for field " . $field, "warning");
            } else {
              _description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value);
            }

        }

      }
    } // END of loop over $derivedUnitFacade fields


    // template_preprocess_description_list() is not worting by weight so we do it right here
    uasort($groups, 'element_sort');

    $occurrence_elements = array(
        '#title' => $title,
        '#theme' => 'description_list',
        '#groups' => $groups,
        '#attributes' => array('class' => html_class_attribute_ref($specimenOrObservation)),
    );
    $derivatives[] = $occurrence_elements;
    // all footnotes which has been assembled so far (e.g. from typeDesignations) to here
    $foonote_li_elements = theme('cdm_footnotes', array('footnoteListKey' => RenderHints::getFootnoteListKey(), 'enclosingTag' => 'span'));
    if (!empty($foonote_li_elements)) {
      $derivatives[] =  array(
          '#markup' =>  '<div class="footnotes">' . $foonote_li_elements . '</div>',
      );
    }

    // --- recurse into originals
    if (!isset($derivedFrom)) {
      $derivedFrom = cdm_ws_get(
          CDM_WS_OCCURRENCE,
          array($specimenOrObservation->uuid, 'derivedFrom')
        );
    }

    if (isset($derivedFrom)) {
      if (isset($derivedFrom->originals)) {
        $derived_from_label = t('derived');
        $preposition = t('from');
        if(isset($derivedFrom->type)){
          $derived_from_label = $derivedFrom->type->representation_L10n;
          if($derivedFrom->type->uuid == UUID_DERIVATIONEVENTTYPE_ACCESSIONING){
            $preposition = t('of');
          }
        }
        if (count($groups) > 0) {
          // TODO  annotations

          // only display the derived from information when the derivative has any element which will be diplayed
          $derivatives[] = array(
              '#markup' => '<div class="derived_from">' . $derived_from_label . ' ' . $preposition . ': </div>',
          );
        }
        foreach ($derivedFrom->originals as $original) {
          compose_cdm_specimenOrObservation($original, $derivatives);
        }
      }
    }

  } // END of $specimenOrObservation exists

  return $derivatives;
}

/**
 * Compose an render array from a CDM Sequence object.
 *
 * compose_hook() implementation
 *
 * @param object $sequence
 *   CDM instance of type Sequence
 * @return array
 *   A render array containing the fields of the supplied $sequence
 *
 * @ingroup compose
 */
function compose_cdm_sequence($sequence) {

  $exclude_sequence_fields = &drupal_static(__FUNCTION__);
  if (!isset($exclude_sequence_fields)) {
    $exclude_sequence_fields = array(
      'titleCache',
      'protectedTitleCache',
      'microReference',
      'created',
      'updated',
      'class',
    );
  }

  $groups = array();

  // -- retrieve additional data if neesscary
  // TODO below call disabled since sequences are not yet supported,
  //      see  #3347 (services and REST service controller for molecular classes implemented)
  //
  // cdm_load_annotations($sequence);

  foreach (get_object_vars($sequence) as $field => $value) {


    if (!in_array($field, $exclude_sequence_fields) && ($value && (!is_object($value) || isset($value->class)))) {
      switch ($field) {

        case 'geneticAccessionNumber';
          $dd_elements = array();
          foreach ($value as $accession) {
            if (isset($accession->uri) ){
              $dd_elements[] = l($accession->accessionNumber, $accession->uri);
            } else {
              $dd_elements[] = $accession->accessionNumber;
            }
          }
          @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),  $dd_elements, NULL, 1);
          break;


        case 'locus': // FIXME 3.3 now dnaMarker (DefinedTerm)  if multiple amplifications where used to build this consensus sequence it may be the super set of the markers used in amplification.
          if (isset($value->name)) {
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),  $value->name, NULL, 3);
          }
          if (isset($value->description)) {
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field) . ' ' . t('description') , $value->description, NULL, 4);
          }
          break;

        case 'consensusSequence':
          // format in genbank style, force linebreaks after each 70 nucleotites
          // see also http://stackoverflow.com/questions/499137/css-how-can-i-force-a-long-string-without-any-blank-to-be-wrapped-in-xul-and
          @_description_list_group_add(
            $groups,
            cdm_occurrence_field_name_label($field),
            array(
              array(
                '#markup' => '<div class="sequence-length">' . $value->length . ' ' . t('pb'). '</div><div>' . wordwrap($value->string, 70, '</br>', TRUE) . '</div>',
                '#wrapper_attributes' => array('class'=>'dna-sequence')
                )
              ),
            5);
          break;

         case 'dnaSample': // FIXME 3.3 implement
            break;
        case 'singleReads': // FIXME 3.3 implement
          break;
        case 'contigFile': // FIXME 3.3 implement - Media
            break;
        case 'pherograms': // FIXME 3.3 implement - Media
          break;
        case 'haplotype': // FIXME 3.3 implement
            break;
        case 'dateSequenced': // FIXME 3.3 now in SingelRead
          @_description_list_group_add($groups, t('Sequencing date'),  timePeriodToString($value), NULL, 6);
          break;

        case 'barcode': // boolean
          @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value ? 'Yes': 'No', NULL, 7);
          break;
        case 'barcodeSequencePart': // FIXME 3.3 implement, compose sequence
            break;

        case 'citation':
          @_description_list_group_add($groups,
            cdm_occurrence_field_name_label($field),
            theme('cdm_reference', array('reference' =>$value, 'microReference' => $sequence->microReference)),
            NULL,
            8
          );
          break;

        case 'publishedIn':
          @_description_list_group_add($groups,
            cdm_occurrence_field_name_label($field),
            theme('cdm_reference', array('reference'=>$value)),
            NULL,
            7
          );

        case 'rights':
          array_merge($groups, cdm_rights_as_dl_groups($value));
        break;

        case 'annotations':
          $dd_elements = array();
          foreach ($value as $annotation) {
            // TODO respect annotation type filter settings
            $dd_elements[] = $annotation->text;
          }
          @_description_list_group_add($groups, t('Notes'), $dd_elements, NULL, 9);
          break;

        case 'markers':
          $dd_elements = array();
          foreach ($value as $marker) {
            $dd_elements[] = compose_cdm_marker($marker);
          }
          @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $dd_elements, NULL, 10);
          break;

        case 'chromatograms':
          @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
              array(
                  '#markup'=>theme('cdm_media_gallerie', array('medialist'=>$value)),
              ),
              NULL,
              11);
          break;

        default:
          if(is_object($value) || is_array($value)){
            drupal_set_message("Unhandled type in compose_cdm_sequence() for field " . $field, "warning");
          } else {
            _description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value, NULL, 20);
          }
      }
    }
  }

  // template_preprocess_description_list() is not worting by weight so we do it right here
  uasort($groups, 'element_sort');

  $sequence_elements = array(
      '#theme' => 'description_list',
      '#groups' => $groups
  );

  return $sequence_elements;
}


