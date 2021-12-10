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
 * Renders a DerivedUnit either in a compact form or with full details.
 *
 * @param $derived_unit_dto
 *  The DerivedUnitDTO
 * @param $with_full_details
 *  Wether to render the dertived unit data with full details, like for example for the specimen page
 *
 * @return string
 *  The markup for a specimen page
 */
function render_cdm_specimenDTO_page($derived_unit_dto, $with_full_details = false)
{
    $detail_html = "";
    //link to specimen page
    $path_to_specimen = path_to_specimen($derived_unit_dto->uuid);
    if (!$with_full_details) {
        $specimenPageLink = l($derived_unit_dto->specimenShortTitle, $path_to_specimen);
        $detail_html .= "<strong>Specimen summary: $specimenPageLink</strong><br>";
    }

    if($with_full_details) {
        if($derived_unit_dto->summaryLabel){
            $detail_html .= "<br>".$derived_unit_dto->summaryLabel."<br>";

        }
    }
    if ($derived_unit_dto->preferredStableUri) {
        $stableIdentifierLink = l($derived_unit_dto->preferredStableUri, $derived_unit_dto->preferredStableUri);
        $detail_html .= create_label("Preferred stable URI") . $stableIdentifierLink . "<br>";
    }
    if ($with_full_details) {
        // associated taxa
        if ($derived_unit_dto->associatedTaxa) {
            $detail_html .= "<br>";
            $detail_html .= create_label("Associated with");
            if (sizeof($derived_unit_dto->associatedTaxa) > 1) {
                $detail_html .= "<br>";
            }
            foreach ($derived_unit_dto->associatedTaxa as $associatedTaxon) {
                $detail_html .= l($associatedTaxon->label, path_to_taxon($associatedTaxon->uuid, "specimens"));//$associatedTaxon->second."<br>";
            }
            $detail_html .= "<br>";
        }
    }
    // - type information
    $types = "";
    if (isset($derived_unit_dto->types)) {
        //typed taxa
        foreach ($derived_unit_dto->types as $typeStatus => $typedTaxa) {
            if($derived_unit_dto->types){
                if(empty($typeStatus) || $typeStatus == "_empty_"|| $typeStatus == ""){
                    $detail_html .= "<i>Type for:</i> ";
                } else {
                    $detail_html .= "<i>".ucfirst($typeStatus)."</i> of ";
                }
                foreach($typedTaxa as $typedTaxon){
                    $detail_html .= $typedTaxon." ";

                }
            } else {
                $types .= $typeStatus . " ";
            }
           $detail_html .= '<br>';
        }
    }
    $derivation_tree_summary_dto = $derived_unit_dto->derivationTreeSummary;
    // - specimen scans
    $specimenScans = create_html_links($derivation_tree_summary_dto->specimenScans, true);
    // - molecular data
    $molecularData = "";
    if ($derivation_tree_summary_dto->molecularDataList) {
        foreach ($derivation_tree_summary_dto->molecularDataList as $molecular) {
            //provider link
            if (isset($molecular->providerLink)) {
                $molecularData .= create_html_link($molecular->providerLink, true);
            } else {
                $molecularData .= "[no provider]";
            }
            //contig link
            if (isset($molecular->contigFiles)) {
                $molecularData .= "[";
                if (sizeof($molecular->contigFiles) > 0) {
                    foreach ($molecular->contigFiles as $contigFile) {
                        if (isset($contigFile->contigLink)) {
                            if (isset($contigFile->contigLink->uri) and $contigFile->contigLink->uri != null) {
                                $molecularData .= create_html_link($contigFile->contigLink, true) . " ";
                            }
                        } else {
                            $molecularData .= "no contig ";
                        }
                        //primer links
                        if (isset($contigFile->primerLinks)) {
                            $molecularData .= create_html_links($contigFile->primerLinks, true);
                        }
                    }
                }
                $molecularData = rtrim($molecularData, " ");
                $molecularData .= "]";
            }
            //FIXME separate with comma (remove trailing comma)
        }
    }
    // - detail images
    $detailImages = create_html_links($derivation_tree_summary_dto->detailImages, true);

    if ($types) {
        $detail_html .= $with_full_details ? "<br>" : "";
        $detail_html .= create_label("Type(s)") . $types . "<br>";
    }
    if ($specimenScans and !$with_full_details) {
        $detail_html .= create_label("Specimen Scans") . $specimenScans . "<br>";
    }
    //specimen scan image gallery
    if ($with_full_details and isset($derivation_tree_summary_dto->specimenScanUuids) and !empty($derivation_tree_summary_dto->specimenScanUuids)) {
        $detail_html .= addImageGallery("Specimen scans", $derivation_tree_summary_dto->specimenScanUuids);
    }

    if ($molecularData) {
        $detail_html .= $with_full_details ? "<br>" : "";
        $detail_html .= create_label("Molecular Data") . $molecularData . "<br>";
    }

    if ($detailImages and !$with_full_details) {
        $detail_html .= create_label("Detail Images") . $detailImages . "<br>";
    }

    //detail image gallery
    if ($with_full_details and isset($derivation_tree_summary_dto->detailImageUuids) and !empty($derivation_tree_summary_dto->detailImageUuids)) {
        $detail_html .= addImageGallery("Detail Images", $derivation_tree_summary_dto->detailImageUuids);
    }

    //character data
    if ($derived_unit_dto->characterData) {
        $detail_html .= $with_full_details ? "<br>" : "";
        $detail_html .= create_label("Character Data");
        if ($with_full_details) {
            $detail_html .= "<br>";
            foreach ($derived_unit_dto->characterData as $characterStatePair) {
                $detail_html .= "<i>" . $characterStatePair->first . "</i>:" . $characterStatePair->second;
                $detail_html .= "<br>";
        }
        } else {
            $detail_html .= l("detail page", $path_to_specimen);
            $detail_html .= "<br>";
        }
    }
    return $detail_html;
}

function addImageGallery($galleryName, $imageUuids)
{
    $images = array();
    foreach ($imageUuids as $uuid) {
        $images[] = cdm_ws_get(CDM_WS_PORTAL_MEDIA, $uuid);
    }

    $gallery_html = '';
    if (count($imageUuids) > 0) {
        $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME);
        $captionElements = array(
            'title',
            'rights',
        );
        $alternativeMediaUris = array();
        foreach ($images as $image) {
            $mediaUri = getMediaUri($image);
            if ($mediaUri) {
                $alternativeMediaUris[] = $mediaUri;
            } else {
                $alternativeMediaUris[] = path_to_media($image->uuid);
            }
        }



        $gallery_html = compose_cdm_media_gallery(array(
            'mediaList' => $images,
            'galleryName' => $galleryName,
            'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
            'cols' => $gallery_settings['cdm_dataportal_media_cols'],
            'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? isset($gallery_settings['cdm_dataportal_media_maxRows']) : null,
            'captionElements' => $captionElements,
            'mediaLinkType' => 'LIGHTBOX',
          // 'alternativeMediaUri' => $alternativeMediaUris,
            'alternativeMediaUri' => NULL,
            'galleryLinkUri' => NULL,
        ));
    }
    return "<br>" . create_label($galleryName) . "<br>" . $gallery_html;
}

function getMediaUri($media)
{
    if (isset($media->representations) && sizeof($media->representations) == 1
        && isset($media->representations[0]->parts) &&
        sizeof($media->representations[0]->parts) == 1) {
        return $media->representations[0]->parts[0]->uri;
    }
    return null;
}


/**
 * Formats the given string to a label for displaying key-object pairs in HTML
 * @return string
 */
function create_label($label)
{
    return "<span class='specimen_table_label'>" . $label . ": </span>";
}

/**
 * Provides a humane representation for an occurrence or observation field name.
 * @param string $field
 *    The field name.
 * @return string
 *    The label for the fieldname.
 */
function cdm_occurrence_field_name_label($field){

  static $field_labels = array(
    'class' => 'Basis of Record',
    'fieldNumber' => 'Collecting number',
    'absoluteElevation' => 'Altitude',
    'absoluteElevationMinimum' => 'Altitude maximum',
    'absoluteElevationMaximum' => 'Altitude minimum',
    'getGatheringPeriod' => 'Gathering period'
  );

  if(str_endsWith($field,'_L10n')){
    $field = str_replace('_L10n', '', $field );
  }
  if (isset($field_labels[$field])) {
    $field = $field_labels[$field];
  }

  $field = preg_replace_callback(
    '/([a-z])([A-Z])/',
    function ($m) {
      return $m[1] . ' ' . strtolower ($m[2]);
    },
    $field);

  return t('@field-name:', array('@field-name' => ucfirst($field)));
}

/**
 * Compose an render array from a CDD SpecimenOrObservation entity.
 *
 * compose_hook() implementation
 *
 * @param object $specimen_or_observation
 *   the CDM FieldUnit or DerivedUnit to compose
 *   the render array for.
 * @param bool $isSpecimen_page
 * @param array $derivatives
 *   the render array which contains the compositions of the derivatives
 *   of the supplied $specimenOrObservation
 *
 * @return array
 *   the supplied render array $derivatives to which the composition of the supplied
 *   $specimenOrObservation has been added to
 *
 * @throws \Exception
 * @ingroup compose
 */

function compose_cdm_specimen_or_observation($specimen_or_observation, $isSpecimen_page = false, &$derivatives = null)
{
    $exclude_occurrence_fields = &drupal_static(__FUNCTION__);
    if (!isset($exclude_occurrence_fields)) {
        $exclude_occurrence_fields = array(
            'uuid',
            'titleCache',
            'protectedTitleCache',
            'derivedUnitMedia',
            'created',
            'publish',
            'updated',
            'class',
            'collectionCode'
        );
    }
    if ((variable_get(CDM_SPECIMEN_LIST_VIEW_MODE, CDM_SPECIMEN_LIST_VIEW_MODE_DEFAULT) == CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TABLE) || !$isSpecimen_page){
        $exclude_occurrence_fields[] = 'derivationEvents';
    }

    if (!isset($derivatives)) {
        $derivatives = array();
    }
    $descriptions = null;
    $derivedFrom = null;

    if (is_object($specimen_or_observation)) {

        // request again for deeper initialization
        $specimen_or_observation = cdm_ws_get("portal/" . CDM_WS_OCCURRENCE, $specimen_or_observation->uuid);
        if ($specimen_or_observation->class == 'FieldUnit'){
            // WARNING: adding a List<MediaDTO> as $specimen_or_observation->_derivedUnitMedia
            $specimen_or_observation->_derivedUnitMedia = cdm_ws_get(CDM_WS_DERIVEDUNIT_FACADE, array(
                $specimen_or_observation->uuid,
                'fieldObjectMediaDTO',
            ));
        }else{
            // WARNING: adding a List<Media> as $specimen_or_observation->_derivedUnitMedia
            $specimen_or_observation->_derivedUnitMedia = cdm_ws_get(CDM_WS_DERIVEDUNIT_FACADE, array(
                $specimen_or_observation->uuid,
                'derivedUnitMedia',
            ));
        }

        $type_label = $specimen_or_observation->class;
        RenderHints::setFootnoteListKey($type_label . '-' . $specimen_or_observation->uuid);

        // collect typeStatus as label
        if (isset($specimen_or_observation->specimenTypeDesignations)) {
            $type_status = array();
            foreach ($specimen_or_observation->specimenTypeDesignations as $typeDesignation) {
                if (isset($typeDesignation->typeStatus_L10n)) {
                    $type_status[] = $typeDesignation->typeStatus_L10n;
                }
            }
            if (count($type_status) > 0) {
                $type_label = implode(', ', $type_status);
            }
        }

        $title = $type_label . ': ' . $specimen_or_observation->titleCache;

        $groups = array();

        // --- add initialized fields
        foreach (get_object_vars($specimen_or_observation) as $field => $value) {
            if (!in_array($field, $exclude_occurrence_fields) && ($value && ($field == 'recordBasis' || !is_object($value) || isset($value->class)))) {
                switch ($field) {

                  case 'recordBasis':
                    $label = $value->representation_L10n;
                    if($label == 'Dna Sample'){
                      $label == 'DNA Sample';
                    }
                    _description_list_group_add($groups, cdm_occurrence_field_name_label($field), $label,NULL, -10);
                    break;

                    /* ---- java.lang.Object --- */
                    case 'class':
                        if ($value != '' /* FieldUnit' */) {
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
                        @_description_list_group_add($groups, t('Notes:'), $dd_elements);
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

                    case 'preferredStableUri':

                        @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), array(array('#markup' => cdm_external_uri($value, false))));
                        break;

                    case 'specimenTypeDesignations':

                        //TODO: better display of details!!
                        @_description_list_group_add(
                            $groups,
                            cdm_occurrence_field_name_label($field),
                            array(
                                '#markup' => render_type_designations($value),
                            )
                        );
                        break;

                    case 'determinations':
                        $dd_elements = array();
                        $glue = ', ';

                        foreach ($value as $determinationEvent) {
                            $timeperiod_string = NULL;
                            if (isset_not_empty($determinationEvent->timeperiod)) {
                                $timeperiod_string = timePeriodToString($determinationEvent->timeperiod);
                            }
                            $weight = isset($determinationEvent->preferred) && $determinationEvent->preferred == 1 ? '0' : ($timeperiod_string ? $timeperiod_string : '1');
                            // check key exists
                            while (array_key_exists($weight, $dd_elements)) {
                                $weight .= '0';
                            }

                            $taxon_name = '';
                            $name_link = '';
                            if ($determinationEvent->taxonName) {
                                $taxon_name = cdm_ws_get(CDM_WS_NAME, $determinationEvent->taxonName->uuid);
                                $taxon_name->taggedName = cdm_ws_get(CDM_WS_NAME, array($determinationEvent->taxonName->uuid, "taggedName"));
                                $name_link = path_to_name($determinationEvent->taxonName->uuid);
                            } else if ($determinationEvent->taxon) {
                                $taxon_name = cdm_ws_get(CDM_WS_TAXON . '/$0/name', $determinationEvent->taxon->uuid);
                                $name_link = path_to_taxon($determinationEvent->taxon->uuid);
                            }
                            if ($taxon_name) {
                                //$taxon_html = render_taxon_or_name($taxon_name, $name_link);

                                $taxon_html = l($taxon_name->titleCache, $name_link);
                                $dd_elements[$weight] = $taxon_html;
                            }
                            if (isset_not_empty($determinationEvent->modifier)) {
                                $dd_elements[$weight] .= cdm_term_representation($determinationEvent->modifier);
                            }
                            if ($timeperiod_string) {
                                $dd_elements[$weight] .= $glue . $timeperiod_string;
                            }
                            if (isset_not_empty($determinationEvent->actor->titleCache)) {
                                $dd_elements[$weight] .= $glue . $determinationEvent->actor->titleCache;
                            }
                            if (isset_not_empty($determinationEvent->description)) {
                                $dd_elements[$weight] .= $glue . $determinationEvent->description;
                            }
                        }
                        ksort($dd_elements);
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label('determinations'), $dd_elements);
                        break;

                    case 'descriptions':
                        $dd_elements = array();
                        foreach ($value as $description) {
                            $description = cdm_ws_get(CDM_WS_PORTAL_DESCRIPTION, $description->uuid);

                            if ($description->imageGallery == TRUE) {
                                continue;
                            }
                            $description_string = render_description_string(get_root_nodes_for_dataset($description));
                            $dd_elements[] = markup_to_render_array($description_string);
                        }
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $dd_elements);
                        break;
                    case '_derivedUnitMedia':
                        if ($isSpecimen_page) {
                            $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME);
                            $captionElements = array(
                                '#uri' => t('open media'),
                            );
                            $gallery_html = compose_cdm_media_gallery(array(
                                'mediaList' => $value, // WARNING: this is either a List<Media> or List<MediaDTO>, see above ~line 361
                                'galleryName' => $specimen_or_observation->titleCache,
                                'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
                                'cols' => $gallery_settings['cdm_dataportal_media_cols'],
                                'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? isset($gallery_settings['cdm_dataportal_media_maxRows']) : null,
                                'captionElements' => $captionElements,
                                'mediaLinkType' => 'LIGHTBOX',
                                'alternativeMediaUri' => NULL,
                                'galleryLinkUri' => NULL,
                            ));
                            @_description_list_group_add($groups, "Detail Images:", markup_to_render_array($gallery_html), '', 20);
                        }

                        // $dd_elements[] = markup_to_render_array($gallery_html);

                        break;
                    case 'sources':
                        RenderHints::setAnnotationsAndSourceConfig([
                          'sources_as_content' => TRUE,
                          'link_to_name_used_in_source' => TRUE,
                          'link_to_reference' => FALSE,
                          'add_footnote_keys' => FALSE,
                          'bibliography_aware' => FALSE
                        ]);
                        $annotations_and_sources = handle_annotations_and_sources($specimen_or_observation);
                        if ($annotations_and_sources->hasSourceReferences()) {
                            @_description_list_group_add($groups, t('Sources') . ':', join(', ', $annotations_and_sources->getSourceReferences()), '', 12);
                        }
                        break;


                    /* ---- DerivedUnitBase --- */
                    case 'derivedFrom':
                        $derivedFrom = $value;
                        if ($isSpecimen_page) {
                            foreach ($derivedFrom->originals as $original) {
                                $pathToSpecimen = path_to_specimen($original->uuid);
                                $description = "";
                                if (isset($derivedFrom->description) && $derivedFrom->description != '') {
                                    $description = $derivedFrom->description . ": ";
                                }

                                $originals[] = markup_to_render_array(l($description . $original->titleCache, $pathToSpecimen));
                                if ($original->class == 'FieldUnit') {
                                    $label = t('Field data');
                                } else {
                                    $label = t('Derived from');
                                }
                                @_description_list_group_add($groups, $label . ':',
                                    $originals,
                                    '', 13);
                            }
                        }
                        break;
                    case 'derivationEvents':
                        $derivationEvents = $value;
                        $derived_units = array();
                        if ($isSpecimen_page) {
                            foreach ($derivationEvents as $derivationEvent) {
                                foreach ($derivationEvent->derivatives as $derived_unit) {
                                    $pathToSpecimen = path_to_specimen($derived_unit->uuid);
                                    $description = "";
                                    if (isset($derived_unit->description) && $derived_unit->description != '') {
                                        $description = $derived_unit->description . ": ";
                                    }

                                    $derived_units[] = markup_to_render_array(l($description . $derived_unit->titleCache, $pathToSpecimen));
                                }
                            }
                            @_description_list_group_add($groups, t('Derivatives') . ':',
                                $derived_units,
                                '', 100);

                        }
                        break;

                    case 'collection':
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), array(
                          array('#markup' => render_collection($value))
                        ));
                        break;

                    case 'storedUnder':
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label('storedUnder'), array(
                          array('#markup' => render_taxon_or_name($value))
                        ));
                        break;
                    case 'dnaQuality':
                        $sub_dl_groups = array();

                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('purificationMethod'), $value->purificationMethod, NULL, 1);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('ratioOfAbsorbance260_230'), $value->ratioOfAbsorbance260_230, NULL, 2);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('ratioOfAbsorbance260_280'), $value->ratioOfAbsorbance260_280, NULL, 3);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('concentration'), $value->concentration, NULL, 4);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('concentrationUnit'), $value->concentrationUnit, NULL, 4);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('qualityTerm'), $value->qualityTerm, NULL, 4);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('qualityCheckDate'), $value->qualityCheckDate, NULL, 4);

                        if (is_array($sub_dl_groups) && sizeof($sub_dl_groups)>0) {
                            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
                                array(
                                    array('#markup' => $value->titleCache),
                                    array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                                )
                            );
                        }
                        break;

                    case 'preservation':
                        $sub_dl_groups = array();

                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('medium'), $value->medium, NULL, 1);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('temperature'), $value->temperature, NULL, 2);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('definedMaterialOrMethod'), $value->definedMaterialOrMethod, NULL, 3);

                        if (is_array($sub_dl_groups) && sizeof($sub_dl_groups)>0) {
                            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
                                array(
                                    array('#markup' => $value->titleCache),
                                    array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                                )
                            );
                        }
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
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label('collector'), $value->actor->titleCache, '', 1);
                        @_description_list_group_add($groups, t('Gathering date'), timePeriodToString($value->timeperiod), '', 2);
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label('description'), $value->description, '', 3);
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label('locality'), $value->locality->text, '', 10);
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label('country'), $value->country->representation_L10n, '', 4);
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label('collectingMethod'), $value->collectingMethod,'',5);
                        if (isset_not_empty($value->absoluteElevation)) {
                            $min_max_markup = statistical_values_from_gathering_event($value, 'absoluteElevation');
                            @_description_list_group_add($groups, cdm_occurrence_field_name_label('absoluteElevation'), $min_max_markup, '',6);
                        }
                        if (isset_not_empty($value->distanceToGround)) {
                            $min_max_markup = statistical_values_from_gathering_event($value, 'distanceToGround');
                            @_description_list_group_add($groups, cdm_occurrence_field_name_label('distanceToGround'), $min_max_markup,'',7);
                        }
                        if (isset_not_empty($value->distanceToWaterSurface)) {
                            $min_max_markup = statistical_values_from_gathering_event($value, 'distanceToWaterSurface');
                            @_description_list_group_add($groups, cdm_occurrence_field_name_label('distanceToWaterSurface'), $min_max_markup, '',8);
                        }
                        if (isset_not_empty($value->collectingAreas)) {
                            $area_representations = array();
                            foreach ($value->collectingAreas as $area) {
                                $area_representations[] = l($area->representation_L10n, path_to_named_area($area->uuid));
                            }
                            @_description_list_group_add($groups, cdm_occurrence_field_name_label('collectingAreas'),
                                array(
                                    array('#markup' => implode(', ', $area_representations))
                                ),'',9
                            );
                        }
                        if (isset_not_empty($value->exactLocation->sexagesimalString)) {
                            @_description_list_group_add($groups, cdm_occurrence_field_name_label('exactLocation'),
                                array(
                                    array('#markup' => render_point($value->exactLocation)),

                                ),'',11
                            );
                        }
                        break;

                    default:
                        if (is_object($value) || is_array($value)) {
                            drupal_set_message("Unhandled type in compose_cdm_specimen_or_observation() for field " . $field, "warning");
                        } else {
                            if ($field == 'RecordBase' && $value == 'DnaSample') {
                                _description_list_group_add($groups, cdm_occurrence_field_name_label($field), 'DNA Sample');
                            } else {
                                _description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value);
                            }
                        }

                }

            }
        } // END of loop over $derivedUnitFacade fields

        // Extensions
        // TODO: filter by using visible_extensions_sorted()
        // TODO: treat as top level element, see https://dev.e-taxonomy.eu/redmine/issues/2985#note-23
        $extensions = cdm_ws_fetch_all(CDM_WS_PORTAL_OCCURRENCE . '/'  . $specimen_or_observation->uuid . '/extensions', array($specimen_or_observation->uuid));
        if ($extensions && count($extensions)) {

            $extensions_render_array = compose_extensions($extensions);
            @_description_list_group_add($groups, t('Extensions') . ':',
                $extensions_render_array,
                '', 100);
        }



        // template_preprocess_description_list() is not worting by weight so we do it right here
        uasort($groups, 'element_sort');

        $occurrence_elements = array(
           // '#title' => $title,
            '#theme' => 'description_list',
            '#groups' => $groups,
            '#attributes' => array('class' => html_class_attribute_ref($specimen_or_observation)),
        );

        $derivatives[] = $occurrence_elements;
        // all footnotes which has been assembled so far (e.g. from typeDesignations) to here
//       $foonote_li_elements = render_footnotes(RenderHints::getFootnoteListKey(), 'span');
//        if (!empty($foonote_li_elements)) {
//            $derivatives[] =  array(
//                '#markup' =>  '<div class="footnotes">' . $foonote_li_elements . '</div>',
//            );
//        }

        // --- recurse into originals
        if (!isset($derivedFrom)  && !$isSpecimen_page) {
            $derivedFrom = cdm_ws_get(
                CDM_WS_OCCURRENCE,
                array($specimen_or_observation->uuid, 'derivedFrom')
            );
        }
        if (isset($derivedFrom) && !$isSpecimen_page) {
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
                    compose_cdm_specimen_or_observation($original, $isSpecimen_page, $derivatives);
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
function compose_cdm_sequence($sequence, $isSpecimenPage = false)
{

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

    // -- retrieve additional data if necessary
    // TODO below call disabled since sequences are not yet supported,
    //      see  #3347 (services and REST service controller for molecular classes implemented)
    //
    // fetch_visible_annotations($sequence);

    foreach (get_object_vars($sequence) as $field => $value) {


        if (!in_array($field, $exclude_sequence_fields) && ($value && (!is_object($value) || isset($value->class)))) {
            switch ($field) {

                case 'geneticAccessionNumber';

                    @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value, NULL, 2);
                    break;


                case 'dnaMarker': // FIXME 3.3 now dnaMarker (DefinedTerm)  if multiple amplifications where used to build this consensus sequence it may be the super set of the markers used in amplification.
                    if (isset($value->name)) {
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value->name, NULL, 3);
                    }
                    if (isset($value->description)) {
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label($field) . ' ' . t('description'), $value->description, NULL, 4);
                    }
                    @_description_list_group_add($groups, cdm_occurrence_field_name_label($field) , $value, NULL, 4);
                    break;

                case 'consensusSequence':
                    // format in genbank style, force linebreaks after each 70 nucleotites
                    // see also http://stackoverflow.com/questions/499137/css-how-can-i-force-a-long-string-without-any-blank-to-be-wrapped-in-xul-and
                    if ($value->length > 0) {
                        @_description_list_group_add(
                            $groups,
                            cdm_occurrence_field_name_label($field),
                            array(
                                array(
                                    '#markup' => '<div class="sequence-length">' . $value->length . ' ' . t('pb') . '</div><div>' . wordwrap($value->string, 70, '</br>', TRUE) . '</div>',
                                    '#wrapper_attributes' => array('class' => 'dna-sequence')
                                )
                            ),
                            5);
                    }
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
                    @_description_list_group_add($groups, t('Sequencing date'), timePeriodToString($value), NULL, 6);
                    break;

                case 'barcode': // boolean
                    @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value ? 'Yes' : 'No', NULL, 7);
                    break;
                case 'barcodeSequencePart': // FIXME 3.3 implement, compose sequence
                    break;

                case 'citation':
                    @_description_list_group_add($groups,
                        cdm_occurrence_field_name_label($field),
                        cdm_reference_markup($value, $sequence->microReference),
                        NULL,
                        8
                    );
                    break;

                case 'publishedIn':
                    @_description_list_group_add($groups,
                        cdm_occurrence_field_name_label($field),
                        theme('cdm_reference', array('reference' => $value)),
                        NULL,
                        7
                    );
                    break;

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
                            '#markup' => compose_cdm_media_gallery(array('medialist' => $value)),
                        ),
                        NULL,
                        11);
                    break;

                default:
                    if (is_object($value) || is_array($value)) {
                        drupal_set_message("Unhandled type in compose_cdm_sequence() for field " . $field, "warning");
                    } else {
                        if (!is_array($value) && strpos($value, 'http:') !== false ){
                            //make links for urls
                            $value = l($value, $value);
                            $value = markup_to_render_array($value);
                        }

                       _description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value, NULL, 20);
                    }
            }
        }
    }

    // template_preprocess_description_list() is not worting by weight so we do it right here
    uasort($groups, 'element_sort');
    if ($isSpecimenPage) {
        $sequence_elements = array(
            '#title' => $sequence->dnaMarker,
            '#theme' => 'description_list',
            '#groups' => $groups
        );
    } else{
        $sequence_elements = array(
            '#title' => $sequence->dnaMarker -> titleCache,
            '#theme' => 'description_list',
            '#groups' => $groups
        );
    }

    return $sequence_elements;
}

/**
 * Creates render array items for FieldUnitDTO or DerivedUnitDTO.
 *
 * @param array $root_unit_dtos
 *     list of SpecimenOrObservationDTOs
 * @return array
 *    An array which can be used in render arrays to be passed to the
 * theme_table() and theme_list().
 */
function specimen_render_array_items(array $root_unit_dtos){

  $render_array_items = array();
  $items = array();

  //we need one more item to contain the items of one level (fieldunit, derivate data etc.)
  foreach ($root_unit_dtos as &$sob_dto) {
    $items['data'] = $sob_dto->label;
    $specimen = compose_cdm_specimen_or_observation_tree_entry($sob_dto);
    $children = array();
    $child = array();
    $child['data'] =$specimen;
    // $children[] = create_specimen_array($specimenOrObservation->derivatives);
    if (isset($sob_dto->derivatives) && sizeof($sob_dto->derivatives) > 0){
      usort($sob_dto->derivatives, 'compare_specimen_or_observation_dtos');
      $child['children']= specimen_render_array_items($sob_dto->derivatives);
    }
    $children[]=$child;
    $items['children'] = $children;
    $render_array_items[] = $items;
  }
  return $render_array_items;
}

/**
 * Composes a compressed derivate table showing all derivatives which
 * stem from a common gathering event.
 *
 * @param $root_unit_uuids array
 *  An array of uuids for cdm root units of the derivation graphs.
 *
 * @return array
 *  A drupal render array for a table
 *
 * @see CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TABLE
 *
 * @ingroup compose
 */

function compose_compressed_specimen_derivate_table($root_unit_uuids) {

  // prepare font icons
  $expand_icon = font_awesome_icon_markup(
    'fa-plus-square-o',
    array(
      'alt' => 'Show details',
      'class' => array('expand_icon')
    )
  );
  $collapse_icon = font_awesome_icon_markup(
    'fa-minus-square-o',
    array(
      'alt' => 'Show details',
      'class' => array('collapse_icon')
    )
  );
  $detail_image_icon = '<img title="Detail Image" src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/detail_image_derivate-16x16-32.png' . '"/>';
  $checked_box_icon = '<img src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/step_done.gif' . '"/>';
  $sequence_icon = '<img title="Molecular Data" src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/sequence_derivate-16x16-32.png' . '"/>';
  $character_data_icon = '<img title="Character Data" src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/character_data_derivate-16x16-32.png' . '"/>';

  $rowcount = 0;
  $rows = array();

  foreach ($root_unit_uuids as $root_unit_uuid) {

    //get derivate hierarchy for the FieldUnit
    $sob_dto = cdm_ws_get(CDM_WS_PORTAL_OCCURRENCE_AS_DTO, array($root_unit_uuid));
    if ($sob_dto) {
      //summary row
      if($sob_dto->class == "FieldUnitDTO"){
        $rows[] = array(
          'id' => 'derivate_summary' . $rowcount, // summary row id
          'class' => array('summary_row'),
          'data' => array(
            array(
              'data' => $expand_icon . $collapse_icon,
              'class' => array('summary_row_cell', 'summary_row_icon', 'expand_column')
            ),
            array(
              'data' => $sob_dto->country,
              'class' => array('summary_row_cell')
            ),
            array(
              'data' => $sob_dto->date,
              'class' => array('summary_row_cell')
            ),
            array(
              'data' => $sob_dto->collectingString,
              'class' => array('summary_row_cell')
            ),
            @array(
              'data' => $sob_dto->collectionStatistics,
              'class' => array('summary_row_cell')
            ),
            array(
              'data' => $sob_dto->hasType ? $checked_box_icon : "",
              'class' => array('summary_row_cell', 'summary_row_icon')
            ),
            array(
              'data' => $sob_dto->hasSpecimenScan ? $checked_box_icon : "",
              'class' => array('summary_row_cell', 'summary_row_icon')
            ),
            array(
              'data' => ($sob_dto->hasDna ? $sequence_icon : "") . " "
                . ($sob_dto->hasDetailImage ? $detail_image_icon : "") . " "
                . ($sob_dto->hasCharacterData ? $character_data_icon : ""),
              'class' => array('summary_row_cell', 'summary_row_icon')
            )
          )
        );
      } else {
        $rows[] = array(
          'id' => 'derivate_summary' . $rowcount, // summary row id
          'class' => array('summary_row'),
          'data' => array(
            array(
              'data' => $expand_icon . $collapse_icon,
              'class' => array('summary_row_cell', 'summary_row_icon', 'expand_column')
            ),
            array(
              'data' => $sob_dto->label,
              'class' => array('summary_row_cell'),
              'colspan' => 5
            ),
            array(
              'data' => $sob_dto->hasSpecimenScan ? $checked_box_icon : "",
              'class' => array('summary_row_cell', 'summary_row_icon')
            ),
            array(
              'data' => ($sob_dto->hasDna ? $sequence_icon : "") . " "
                . ($sob_dto->hasDetailImage ? $detail_image_icon : "") . " "
                . ($sob_dto->hasCharacterData ? $character_data_icon : ""),
              'class' => array('summary_row_cell', 'summary_row_icon')
            )
          )
        );
      }

      //assemble field unit details
      $detail_html = "";
      if ($sob_dto->summaryLabel) {
        $detail_html .= create_label("Citation") . $sob_dto->summaryLabel . "<br>";
      }
      //assemble specimen details
      if ($sob_dto->derivatives) {
        $derivatives_orderd = [];
        foreach ($sob_dto->derivatives as $derivative_dto) {
          $derivatives_orderd[$derivative_dto->specimenShortTitle] = $derivative_dto;
        }
        ksort($derivatives_orderd);
        foreach ($derivatives_orderd as $derivative_dto) {
          $detail_html .= "<br>";
          $detail_html .= render_cdm_specimenDTO_page($derivative_dto);
        }
      }
      $detail_html .= "<br>";
      //detail row resp. one BIG detail cell
      $rows[] = array(
        'data' => array(
          array(
            'data' => "", //empty first column
            'class' => array('expand_column')
          ),
          array(
            'data' => $detail_html,
            'colspan' => 7,
          ),
        ),
        'id' => 'derivate_details' . $rowcount,//details row ID
        'class' => array('detail_row'),
      );
      $rowcount++;
    }
  }

  $tableId = "derivate_hierarchy_table";
  $derivateHierarchyTable = array(
    "#theme" => "table",
    "#weight" => 2,
    "#header" => array(
      array(
        'data' => "",
        'class' => array('expand_column')
      ),
      "Country", "Date", "Collector + collecting number", "Herbaria", "Type", "Scan", "Derivatives"),
    "#rows" => $rows,
    "#attributes" => array(
      "id" => $tableId,
      "border" => 2
    )
  );

  //add toggle functionality to derivate hierarchy table
  drupal_add_js_rowToggle("#" . $tableId);

  return $derivateHierarchyTable;
}


/**
 * Renders a FieldUnitDTO or DerivedUnitDTO
 *
 * @param object $specimen_or_observation_dto
 *   A FieldUnitDTO or DerivedUnitDTO object
 *   the render array for
 *
 * @return string
 *   the supplied render array $derivatives to which the composition of the
 *   supplied
 *   $specimenOrObservation has been added to
 *
 * @ingroup compose
 *
 * @throws \Exception
 */
function compose_cdm_specimen_or_observation_tree_entry($specimen_or_observation_dto) {
  $exclude_occurrence_fields = &drupal_static(__FUNCTION__);
  if (!isset($exclude_occurrence_fields)) {
    $exclude_occurrence_fields = [
      'uuid',
      'titleCache',
      'protectedTitleCache',
      'label',
      'class',
      'type',
      'types',
      'taxonRelatedDerivedUnits',
      'collectionCode',
      'derivatives',
      'derivateDataDTO',
      'summaryLabel',
      'derivationTreeSummary',
      'specimenIdentifier', // combination like  collection code + accession number
      'mostSignificantIdentifier',
      // -----
      // needed later on?
      'hasDetailImage',
      'hasType',
      'hasSpecimenScan',
      'collectionStatistics',
      'collectingString'
      // ----
    ];
  }

  if (is_object($specimen_or_observation_dto)) {

    if(isset($specimen_or_observation_dto->recordBase)){
        $type_label = $specimen_or_observation_dto->recordBase->representation_L10n;
    }
      RenderHints::setFootnoteListKey($type_label . '-' . $specimen_or_observation_dto->uuid);

      // collect typeStatus as label
      if (isset($specimen_or_observation_dto->specimenTypeDesignations)) {
          $type_status = array();
          foreach ($specimen_or_observation_dto->specimenTypeDesignations as $typeDesignation) {
              if (isset($typeDesignation->typeStatus_L10n)) {
                  $type_status[] = $typeDesignation->typeStatus_L10n;
              }
          }
          if (count($type_status) > 0) {
              $type_label = implode(', ', $type_status);
        }
      }

      if (isset($typeDesignation->typifiedNames)){
          $title = $type_label . ' for: ' . $typeDesignation->typifiedNames;
      }else{
          $title = $type_label;
      }

        $groups = array();
        $children_items = array();
        // --- add initialized fields
        foreach (get_object_vars($specimen_or_observation_dto) as $field => $value) {
            $child_item = array();

            if (!in_array($field, $exclude_occurrence_fields) && $value ) {
              switch ($field) {

                /* ---- SpecimenOrObservationBase --- */
                case 'recordBase':
                  $label = $value->representation_L10n;
                  if($label == 'Dna Sample'){
                    $label == 'DNA Sample';
                  }
                  _description_list_group_add($groups, cdm_occurrence_field_name_label($field), $label,NULL, -10);
                break;
                case 'derivationEvent':
                    @_description_list_group_add($groups, 'Gathering type:', ucfirst($value->eventType), NULL, 1);
                    break;
                case 'accessionNumber':
                    @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value, NULL, 0);
                    break;
                case 'preferredStableUri':
                    @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), array(array('#markup' => cdm_external_uri($value, false))));
                    break;
                case 'characterData':
                    @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), array(array('#markup' => icon_link(path_to_specimen($specimen_or_observation_dto->uuid), '', FALSE), false)));
                  break;
                case 'specimenTypeDesignations':
                    @_description_list_group_add(
                        $groups,
                        cdm_occurrence_field_name_label($field),
                        array(
                            '#markup' => render_specimen_typedesignation_dto($value),
                        )
                    );
                    break;

                case 'listOfMedia':
                    $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME);

                    $captionElements = array(
                        'title',
                        '#uri' => t('open media'),
                    );
                    $gallery_html = compose_cdm_media_gallery(array(
                        'mediaList' => $value,
                        'galleryName' => $specimen_or_observation_dto->uuid,
                        'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
                        'cols' => $gallery_settings['cdm_dataportal_media_cols'],
                        'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? isset($gallery_settings['cdm_dataportal_media_maxRows']) : null,
                        'captionElements' => $captionElements,
                        'mediaLinkType' => 'LIGHTBOX',
                        'alternativeMediaUri' => NULL,
                        'galleryLinkUri' => NULL,
                        'showCaption' => true
                    ));

                  //FIXME re-enable: // @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $gallery_html);
                  break;

                  case 'sex':
                  case 'lifeStage':
                  case 'kindOfUnit':
                    @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), cdm_term_representation($value));
                    break;

                  case 'definition':
                    // TODO
                    break;

                  /* ---- DerivedUnitBase --- */
                  case 'collection':
                    @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), array(
                      array('#markup' => render_collection_dto($value))
                    ));
                    break;

                  /* ---- Specimen --- */
                  case 'sequences':
                      $dd_elements = array();
                      foreach ($value as $sequence) {
                          $dd_elements[] = compose_cdm_sequence($sequence, true);
                          $dd_elements[] = "";
                      }
                      @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $dd_elements,'', 100);
                      break;

                  // TODO preservation
                  case 'storedUnder':
                    $taxon_name = cdm_ws_get(CDM_WS_PORTAL_NAME, array($value->uuid));
                    @_description_list_group_add($groups, cdm_occurrence_field_name_label('storedUnder'), array(
                      array('#markup' => render_taxon_or_name($taxon_name, path_to_name($taxon_name->uuid)))
                    ));
                  break;

                  /* ---- FieldObservation --- */
                  case 'gatheringEvent':

                      @_description_list_group_add($groups, cdm_occurrence_field_name_label('collector'), $value->collector);
                      @_description_list_group_add($groups, t('Gathering date'), timePeriodToString($value->timeperiod));
                      @_description_list_group_add($groups, cdm_occurrence_field_name_label('description'), $value->description);
                      @_description_list_group_add($groups, cdm_occurrence_field_name_label('locality'), $value->locality, '', 10);
                      @_description_list_group_add($groups, cdm_occurrence_field_name_label('country'), $value->country);
                      @_description_list_group_add($groups, cdm_occurrence_field_name_label('collectingMethod'), $value->collectingMethod);
                      if (isset($value->absoluteElevation)) {
                          $min_max_markup = statistical_values_from_gathering_event($value, 'absoluteElevation');
                          @_description_list_group_add($groups, cdm_occurrence_field_name_label('absoluteElevation'), $min_max_markup);
                      }
                      if (isset($value->distanceToGround) && $value->distanceToGround >0) {
                          $min_max_markup = statistical_values_from_gathering_event($value, 'distanceToGround');
                          @_description_list_group_add($groups, cdm_occurrence_field_name_label('distanceToGround'), $min_max_markup);
                      }
                      if (isset($value->distanceToWaterSurface) && $value->distanceToWaterSurface > 0) {
                          $min_max_markup = statistical_values_from_gathering_event($value, 'distanceToWaterSurface');
                          @_description_list_group_add($groups, cdm_occurrence_field_name_label('distanceToWaterSurface'), $min_max_markup);
                      }

                      if (isset($value->collectingAreas)) {
                          $area_representations = array();
                          foreach ($value->collectingAreas as $area) {
                             // $area_representations[] = l($area->representation_L10n, path_to_named_area($area->uuid));
                              $area_representations[] = $area;
                          }
                          if (!empty($area_representations))
                              @_description_list_group_add($groups, cdm_occurrence_field_name_label('collectingAreas'),
                                  array(
                                      array('#markup' => implode(', ', $area_representations))
                                  )
                              );
                      }
                      if (isset($value->exactLocation)  ) {
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label('exactLocation'),
                          array(
                            array('#markup' => render_point($value->exactLocation)),
                          ),'',11
                        );
                      }
                      break;

                  /* ---- DerivationEvent --- */
                  case 'derivationEvents':
                    //@_description_list_group_add($groups, t('Association type'), $value->description);
                    break;
                  case 'determinedNames':
                    $dd_elements = array();
                    foreach ($value as $name) {
                      $taxon_name = cdm_ws_get(CDM_WS_PORTAL_NAME, $name->uuid);
                      $dd_elements[] = render_taxon_or_name($taxon_name, url(path_to_name($taxon_name->uuid)));
                    }
                    @_description_list_group_add($groups, cdm_occurrence_field_name_label("Identification(s)"), $dd_elements,'', 100);
                    break;

                  default:
                    if (is_object($value) || is_array($value)) {
                        drupal_set_message("Unhandled type in compose_cdm_specimen_or_observation_tree_entry() for field " . $field, "warning");
                    } else {
                      _description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value);
                    }
              }
            }
        } // END of loop over $derivedUnitFacade fields

        // template_preprocess_description_list() is not worting by weight so we do it right here
        uasort($groups, 'element_sort');
        $occurrence_elements = array(
           // '#title' => $title,
            '#theme' => 'description_list',
            '#groups' => $groups,
            '#attributes' => array('class' => html_class_attribute_ref($specimen_or_observation_dto)),
        );
        $output = drupal_render($occurrence_elements);
        if (isset($gallery_html)){
            $output .= $gallery_html;
        }
        $pathToSpecimen = path_to_specimen($specimen_or_observation_dto->uuid);
        $output .=  l("Detail page", $pathToSpecimen);
    } // END of $specimenOrObservation exists

    return $output;
}

function compose_table_of_blast_result(array $data){
   // get icon images
  $expand_icon = font_awesome_icon_markup(
      'fa-plus-square-o',
      array(
          'alt' => 'Show details',
          'class' => array('expand_icon')
      )
  );
  $collapse_icon = font_awesome_icon_markup(
      'fa-minus-square-o',
      array(
          'alt' => 'Show details',
          'class' => array('collapse_icon')
      )
  );
  //$detail_image_icon = '<img title="Detail Image" src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/detail_image_derivate-16x16-32.png' . '"/>';
    $rowcount = 0;
    $rows = array();

    foreach ($data as $row_data){
        $pathToSpecimen = path_to_specimen_by_accession_number($row_data['id']);
        $specimenPageLink = l($row_data['id'], $pathToSpecimen);
        $ncbiLink = l($row_data['id'], 'https://www.ncbi.nlm.nih.gov/nuccore/'.$row_data['id'].'?report=graph');
        $rows[] =  array(
            'data' => array(
                array(
                    'data' => $expand_icon . $collapse_icon,
                    'class' => array('summary_row_cell', 'summary_row_icon', 'expand_column')
                ),
                array(
                    'data' => $row_data['def'],
                ),
                array(
                    'data' => $specimenPageLink,
                ),
                array(
                    'data' => $row_data['hsp_align_length'],
                ),
                array(
                   'data' => $ncbiLink,
                ),
                array(
                    'data' => $row_data['hsp_identity']/$row_data['hsp_align_length']*100,
                )
            ),
            'id' => 'blast_summary' . $rowcount, // summary row id
            'class' => array('summary_row'),
        );
        $detail_html = "";

        $detail_html .= create_label("Details") . $row_data["hsp_align_length"] . "<br>";


        $detail_html .= "<br>";
        $detail_html .= $row_data["hsp_midline"];

        //$detail_html .= "<br>";

        $rows[] = array(
            'data' => array(
                array(
                    'data' => "", //empty first column
                    'class' => array('expand_column')
                ),
                array(
                    'data' => $detail_html,
                    'colspan' => 5,
                ),
            ),
            'id' => 'blast_detail' . $rowcount,//details row ID
            'class' => array('detail_row'),
        );
        $rowcount++;
    }

    $tableId = "blast_result_table";
    $blast_result_table = array(
        "#theme" => "table",
        "#weight" => 2,
        "#header" => array(
            array(
                'data' => "",
                'class' => array('expand_column')
            ),
                        "Name", "Accession Number", "Align Length", "NCBI", "% Identity"),
        "#rows" => $rows,
        "#attributes" => array(
            "id" => $tableId,
            "border" => 2
        )
    );

    drupal_add_js_rowToggle("#".$tableId);

    $render_array[$tableId] = $blast_result_table;
    $out = drupal_render($render_array);
   //$blast_result_page-> content = drupal_render($render_array);
   return $out;
}


/**
 * Composes a BOTTOM-UP-SPECIMEN-TABLE for the view mode
 * CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE
 *
 * @param $specimensOrObservations
 * @return array
 *  A drupal render array with the following keys:
 *   - 'specimen_list'
 *   - 'pager'
 *
 * @ingroup Compose
 * @see CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE
 */
function compose_specimens_table_bottom_up($specimensOrObservations)
{

  // --- generate the specimen list as table
  $specimen_table = array(
    '#theme' => 'table',
    '#weight' => 2,
    // prefix attributes and rows with '#' to let it pass toF the theme function,
    // otherwise it is handled as child render array
    '#attributes' => array('class' => 'specimens '),
    '#rows' => array(),
  );

  if ($specimensOrObservations) {

    foreach ($specimensOrObservations as $specimenOrObservation) {

      $mediaList = array();
      if (is_array($specimenOrObservation->_fieldObjectMedia)) {
        $mediaList = array_merge($mediaList, $specimenOrObservation->_fieldObjectMedia);
      }
      if (is_array($specimenOrObservation->_derivedUnitMedia)) {
        $mediaList = array_merge($mediaList, $specimenOrObservation->_derivedUnitMedia);
      }


      // typelabel will contain the typeStatus
      $type_label = '';
      $typeDesignationPager = cdm_ws_get(CDM_WS_OCCURRENCE . '/$0/specimenTypeDesignations', $specimenOrObservation->uuid);
      if (isset($typeDesignationPager) and isset($typeDesignationPager->records)) {
        $type_status = array();
        foreach ($typeDesignationPager->records as $typeDesignation) {
          if (isset($typeDesignation->typeStatus->representation_L10n)) {
            $type_status[] = $typeDesignation->typeStatus->representation_L10n;
          }
        }
        $type_label = implode(', ', $type_status);
        if ($type_label) {
          $type_label = ucfirst($type_label) . ': ';
        }
      }

      // --- Specimen entry as dynamic label:
      //     -> Dynabox for the specimenOrObservation
      $gallery_name = $specimenOrObservation->uuid;

      $derived_unit_ws_request = cdm_compose_ws_url(CDM_WS_OCCURRENCE, array($specimenOrObservation->uuid));
      // --- Render associated media.
      $gallery_html = '';
      if (count($mediaList) > 0) {
        $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME);
        $captionElements = array(
          '#uri' => t('open media'),
        );

        $gallery_html = compose_cdm_media_gallery(array(
          'mediaList' => $mediaList,
          'galleryName' => $gallery_name,
          'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
          'cols' => $gallery_settings['cdm_dataportal_media_cols'],
          'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? isset($gallery_settings['cdm_dataportal_media_maxRows']) : null,
          'captionElements' => $captionElements,
          'mediaLinkType' => 'LIGHTBOX',
          'alternativeMediaUri' => NULL,
          'galleryLinkUri' => NULL,
        ));
      }
      //here we should use the data we already have
      $label_html = cdm_dynabox(
        $specimenOrObservation->uuid,
        $type_label . $specimenOrObservation->titleCache,
        $derived_unit_ws_request,
        'cdm_specimen_or_observation',
        'Click for details',
        array('div', 'div'),
        array(),
        null, // $content_element_selector
        'function(){ jQuery(\'#media_gallery_' . $gallery_name . '\').hide(); }', // open_callback
        'function(){ jQuery(\'#media_gallery_' . $gallery_name . '\').show(); }' // close_callback
      );

      // --- Render associated media.
      $gallery_html = '';
      if (count($mediaList) > 0) {
        $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME);
        $captionElements = array(
          '#uri' => t('open media'),
        );

        $gallery_html = compose_cdm_media_gallery(array(
          'mediaList' => $mediaList,
          'galleryName' => $gallery_name,
          'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
          'cols' => $gallery_settings['cdm_dataportal_media_cols'],
          'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? isset($gallery_settings['cdm_dataportal_media_maxRows']) : null,
          'captionElements' => $captionElements,
          'mediaLinkType' => 'LIGHTBOX',
          'alternativeMediaUri' => NULL,
          'galleryLinkUri' => NULL,
        ));
      }

      $specimen_table['#rows'][] = array(
        // An array of table rows. Every row is an array of cells, or an associative array
        'data' => array(
          // Each cell can be either a string or an associative array
          $label_html . $gallery_html
        ),
        'class' => array(
          'descriptionElement',
          'descriptionElement_IndividualsAssociation'
        ),
      );
    }
  }

  return $specimen_table;
}


/**
 * Orders occurrences by date but types should be on top of the list.
 *
 * @param array $specimens_or_observations
 *  Array of SpecimenOrObservation or fieldUnitDTOs
 *
 * @return array
 */
function order_specimens_or_observations_by_date_and_type($specimens_or_observations)
{
  $type_specimens = array();
  $other_occurrences = array();
  if(is_array($specimens_or_observations)){
    foreach ($specimens_or_observations as &$occurrence) {
      $typeDesignationsPager = cdm_ws_get(CDM_WS_OCCURRENCE . '/$0/specimenTypeDesignations', $occurrence->uuid);
      if (isset($typeDesignationsPager->count) && $typeDesignationsPager->count > 0) {
        $type_specimens[] = $occurrence;
      } else {
        $other_occurrences[] = $occurrence;
      }
    }
  }
  $specimens_or_observations = array_merge($type_specimens, $other_occurrences);
  return $specimens_or_observations;
}

/**
 * Orders FieldUnitDTOs by date but types should be on top of the list.
 *
 * Delegates internally to order_specimens_or_observations_by_date_and_type()
 *
 * @param array $fieldUnitDTOs
 *
 * @return array
 */
function order_fieldUnitDtos_by_date_and_type($fieldUnitDTOs)
{
  $units_with_types = [];
  $units_no_types = [];

  foreach($fieldUnitDTOs as $dto){
    if(isset($dto->specimenTypeDesignations[0])){
      $units_with_types[] = $dto;
    } else {
      $units_no_types[] = $dto;
    }
  }
  usort($units_with_types, 'compare_specimen_or_observation_dtos_by_date');
  usort($units_no_types, 'compare_specimen_or_observation_dtos_by_date');

  return array_merge($units_with_types, $units_no_types);
}

/**
 * @param $specimen
 *
 * @return array
 */
function specimen_id_and_collection_for($specimen) {
  $specimenID = '';
  $collection = NULL;
  if (!($specimen->class == 'FieldUnit')) {
    if ($specimen->collection) {
      if ($specimen->collection->code) {
        $collection = $specimen->collection->code;
      }
      elseif ($specimen->collection->name) {
        $collection = $specimen->collection->name;
      }
    }
    if ($specimen->accessionNumber) {
      $specimenID = $specimen->accessionNumber;
    }
    elseif ($specimen->barcode) {
      $specimenID = $specimen->barcode;
    }
    elseif ($specimen->catalogNumber) {
      $specimenID = $specimen->catalogNumber;
    }
    elseif ($specimen->titleCache) {
      $specimenID = $specimen->titleCache;
    }
    if (!isset($specimenID) and !isset($collection)) {
      $specimenID = $specimen->uuid;
    }
  }
  else {
    if ($specimen->titleCache) {
      $specimenID = $specimen->titleCache;
    }
    if (!isset($specimenID) and !isset($collection)) {
      $specimenID = $specimen->uuid;
    }
  }
  return [$specimenID, $collection];
}

/**
 * Renders a cdm collection entity as html markup.
 *
 * institute and super-collections
 *
 * @param $collection_dto
 *   The CollectionDTO

 * @return string
 *  The markup for the collection,
 */
function render_collection_dto($collection_dto){
  return render_collection($collection_dto);
}

/**
 * Renders a cdm collection entity as html markup.
 *
 * institute and super-collections
 *
 * @param $collection
 *   The CDM Collection entity or CollectionDTO
 * @param bool $do_link
 *   Append a link to the collection page as clickable icon (default = true).
 *
 * @return string
 *  The markup for the collection,
 */
function render_collection($collection, $do_link = false /* to be made true, see #9250 */ ){
  if(!is_object($collection)){
    return '';
  }
  $is_dto = $collection->class == 'CollectionDTO';
  $collection_str_list = [];
  $super_collection = $collection;
  while($super_collection){
    $collection_str = $is_dto ? $super_collection->label : $super_collection->titleCache;
    if(
    ($is_dto && $super_collection->institute) ||
    (isset_not_empty($super_collection->institute) && isset_not_empty($super_collection->institute->titleCache))){
      $collection_str .= ' at ' . ($is_dto ? $super_collection->institute : $super_collection->institute->titleCache);
    }
    $collection_str_list[] = $collection_str;
    if(isset($super_collection->superCollection)){
      $super_collection = $super_collection->superCollection;
    } else {
      $super_collection = null;
    }
  }
  $markup = join(' in ', $collection_str_list);
  if($markup){
    if($do_link){
      $markup .= ' ' . cdm_internal_link(path_to_collection($collection->uuid));
    }
    $markup = '<span class="' . html_class_attribute_ref($collection) . '">' . $markup . '</span>';
  }
  return $markup;
}

/**
 * Compares two SpecimenTypeDesignations by identifier and label
 *
 * @param object $a
 *   A SpecimenOrObservationDTO.
 * @param object $b
 *   The SpecimenOrObservationDTO.
 */
function compare_specimen_or_observation_dtos($a, $b) {
    // Sort alphabetically.
    $a_text =  isset($a->specimenShortTitle) ? $a->specimenShortTitle : $a->label;
    $b_text =  isset($b->specimenShortTitle) ? $b->specimenShortTitle : $b->label;
    return strcasecmp($a_text, $b_text);
}

/**
 * Compares two SpecimenTypeDesignations by date or label
 *
 * @param object $a
 *   A SpecimenOrObservationDTO.
 * @param object $b
 *   The SpecimenOrObservationDTO.
 */
function compare_specimen_or_observation_dtos_by_date($a, $b) {
  // Sort alphabetically.
  $a_text =  isset($a->date) ? $a->date : $a->label;
  $b_text =  isset($b->date) ? $b->date : $b->label;
  return strcasecmp($a_text, $b_text);
}
