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
 * Provides the HTML markup for a specimen page
 *
 * @param $specimen
 *
 * @return string
 *  The markup for a specimen page
 */
function render_cdm_specimen_page($specimen)
{

    $specimen_details = compose_cdm_specimen_or_observation($specimen, true);
    //$detail_html .= drupal_render($specimen_details);

    $specimen->_fieldObjectMedia = cdm_ws_get(CDM_WS_DERIVEDUNIT_FACADE, array(
        $specimen->uuid,
        'fieldObjectMediaDTO',
    ));
    $specimen->_derivedUnitMedia = cdm_ws_get(CDM_WS_DERIVEDUNIT_FACADE, array(
        $specimen->uuid,
        'derivedUnitMedia',
    ));
    $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME);
    $captionElements = array(
        'title',
        '#uri' => t('open media'),
    );
    if (isset($specimen->fieldObjectMediaDTO) ) {
        $gallery_html = compose_cdm_media_gallerie(array(
            'mediaList' => $specimen->fieldObjectMediaDTO,
            'galleryName' => $specimen->titleCache,
            'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
            'cols' => $gallery_settings['cdm_dataportal_media_cols'],
            'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? isset($gallery_settings['cdm_dataportal_media_maxRows']) : null,
            'captionElements' => $captionElements,
            'mediaLinkType' => 'LIGHTBOX',
            'alternativeMediaUri' => NULL,
            'galleryLinkUri' => NULL,
        ));
        $specimen_details[] = markup_to_render_array($gallery_html);
    }

    if (isset($specimen->_derivedUnitMedia) ) {
        $gallery_html = compose_cdm_media_gallerie(array(
            'mediaList' => $specimen->_derivedUnitMedia,
            'galleryName' => $specimen->titleCache,
            'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
            'cols' => $gallery_settings['cdm_dataportal_media_cols'],
            'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? isset($gallery_settings['cdm_dataportal_media_maxRows']) : null,
            'captionElements' => $captionElements,
            'mediaLinkType' => 'LIGHTBOX',
            'alternativeMediaUri' => NULL,
            'galleryLinkUri' => NULL,
        ));
        $specimen_details[] = markup_to_render_array($gallery_html);
    }

    $specimen_array = $specimen_details;
    $output = "";
    foreach($specimen_array as $value){
        $renderArray = array(
            '#theme' => 'item_list',
            '#items' => array($value),
            '#type' => 'ul');
        $output .= drupal_render($renderArray);
    }


    return $output;
}


/**
 * Provides the HTML markup for a specimen page
 *
 * @param $specimen
 *
 * @return string
 *  The markup for a specimen page
 */
function render_cdm_specimenDTO_page($specimen, $is_specimen_page = false)
{
    $detail_html = "";
    //link to specimen page
    $pathToSpecimen = path_to_specimen($specimen->uuid);
    if (!$is_specimen_page) {
        $specimenPageLink = l($specimen->accessionNumber, $pathToSpecimen);
        $detail_html .= "<strong>Specimen summary: $specimenPageLink</strong><br>";
    }


    if($is_specimen_page) {
        if($specimen->citation){
            $detail_html .= "<br>".$specimen->citation."<br>";

        }
    }
    if ($specimen->preferredStableUri) {
        $stableIdentifierLink = l($specimen->preferredStableUri, $specimen->preferredStableUri);
        $detail_html .= create_label("Preferred stable URI") . $stableIdentifierLink . "<br>";
    }
    if ($is_specimen_page) {
        // associated taxa
        if ($specimen->associatedTaxa) {
            $detail_html .= "<br>";
            $detail_html .= create_label("Associated with");
            if (sizeof($specimen->associatedTaxa) > 1) {
                $detail_html .= "<br>";
            }
            foreach ($specimen->associatedTaxa as $associatedTaxon) {
                $detail_html .= l($associatedTaxon->value, path_to_taxon($associatedTaxon->key, "specimens"));//$associatedTaxon->second."<br>";
            }
            $detail_html .= "<br>";
        }
    }
    // - type information
    $types = "";
    if (isset($specimen->types)) {
        //typed taxa
        foreach ($specimen->types as $typeStatus => $typedTaxa) {
            if($specimen->types){
                if(empty($typeStatus) || $typeStatus == "_empty_"|| $typeStatus == ""){
                    $detail_html .= "<i>Type for:</i> ";
                } else {
                    $detail_html .= "<i>".$typeStatus."</i> of ";
                }
                foreach($typedTaxa as $typedTaxon){
                    $detail_html .= $typedTaxon." ";

                }
            } else {
                $types .= $typeStatus . " ";

            }
        }
    }
    $derivateDataDTO = $specimen->derivateDataDTO;
    // - specimen scans
    $specimenScans = create_html_links($derivateDataDTO->specimenScans, true);
    // - molecular data
    $molecularData = "";
    if ($derivateDataDTO->molecularDataList) {
        foreach ($derivateDataDTO->molecularDataList as $molecular) {
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
    $detailImages = create_html_links($derivateDataDTO->detailImages, true);

    if ($types) {
        $detail_html .= $is_specimen_page ? "<br>" : "";
        $detail_html .= create_label("Type(s)") . $types . "<br>";
    }
    if ($specimenScans and !$is_specimen_page) {
        $detail_html .= create_label("Specimen Scans") . $specimenScans . "<br>";
    }
    //specimen scan image gallery
    if ($is_specimen_page and isset($derivateDataDTO->specimenScanUuids) and !empty($derivateDataDTO->specimenScanUuids)) {
        $detail_html .= addImageGallery("Specimen scans", $derivateDataDTO->specimenScanUuids);
    }

    if ($molecularData) {
        $detail_html .= $is_specimen_page ? "<br>" : "";
        $detail_html .= create_label("Molecular Data") . $molecularData . "<br>";
    }

    if ($detailImages and !$is_specimen_page) {
        $detail_html .= create_label("Detail Images") . $detailImages . "<br>";
    }

    //detail image gallery
    if ($is_specimen_page and isset($derivateDataDTO->detailImageUuids) and !empty($derivateDataDTO->detailImageUuids)) {
        $detail_html .= addImageGallery("Detail Images", $derivateDataDTO->detailImageUuids);
    }

    //character data
    if ($specimen->characterData) {
        $detail_html .= $is_specimen_page ? "<br>" : "";
        $detail_html .= create_label("Character Data");
        if ($is_specimen_page) {
            $detail_html .= "<br>";
            foreach ($specimen->characterData as $characterStatePair) {
                $detail_html .= "<i>" . $characterStatePair->first . "</i>:" . $characterStatePair->second;
                $detail_html .= "<br>";
            }
        } else {
            $detail_html .= l("detail page", $pathToSpecimen);
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



        $gallery_html = compose_cdm_media_gallerie(array(
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

function compose_cdm_specimen_or_observation($specimen_or_observation, $isSpecimen_page = false, &$derivatives = null)
{
    $exclude_occurrence_fields = &drupal_static(__FUNCTION__);
    if (!isset($exclude_occurrence_fields)) {
        $exclude_occurrence_fields = array(

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
    if ((variable_get(CDM_SPECIMEN_LIST_VIEW_MODE, CDM_SPECIMEN_LIST_VIEW_MODE_DEFAULT) == CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TABLE) || !$isSpecimen_page){
        $exclude_occurrence_fields[] = 'derivationEvents';
    }


    // only show uuid it the user is logged in
 //   if(user_is_logged_in() && ($a_idx = array_search('uuid', $exclude_occurrence_fields)) !== FALSE ) {
  //      unset($exclude_occurrence_fields[$a_idx]);
  //  }

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
                if (isset($typeDesignation->typeStatus->representation_L10n)) {
                    $type_status[] = $typeDesignation->typeStatus->representation_L10n;
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
            if (!in_array($field, $exclude_occurrence_fields) && ($value && (!is_object($value) || isset($value->class)))) {
                switch ($field) {

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
                            if (isset($determinationEvent->timeperiod)) {
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
                            $gallery_html = compose_cdm_media_gallerie(array(
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
                        if (!empty($annotations_and_sources['source_references'])) {
                            @_description_list_group_add($groups, t('Sources') . ':', join(', ', $annotations_and_sources['source_references']), '', 12);
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
                            @_description_list_group_add($groups, t('Derivates') . ':',
                                $derived_units,
                                '', 100);

                        }
                        break;

                    case 'collection':
                        $sub_dl_groups = array();
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('code'), $value->code, NULL, 1);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('codeStandard'), $value->codeStandard, NULL, 2);
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
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label('storedUnder'), render_taxon_or_name($value));
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
                        if (isset($value->absoluteElevation)) {
                            $min_max_markup = statistical_values_from_gathering_event($value, 'absoluteElevation');
                            @_description_list_group_add($groups, cdm_occurrence_field_name_label('absoluteElevation'), $min_max_markup, '',6);
                        }
                        if (isset($value->distanceToGround)) {
                            $min_max_markup = statistical_values_from_gathering_event($value, 'distanceToGround');
                            @_description_list_group_add($groups, cdm_occurrence_field_name_label('distanceToGround'), $min_max_markup,'',7);
                        }
                        if (isset($value->distanceToWaterSurface)) {
                            $min_max_markup = statistical_values_from_gathering_event($value, 'distanceToWaterSurface');
                            @_description_list_group_add($groups, cdm_occurrence_field_name_label('distanceToWaterSurface'), $min_max_markup, '',8);
                        }
                        if (isset($value->collectingAreas) && count($value->collectingAreas) > 0) {
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
                        if (isset($value->exactLocation) && $value->exactLocation->sexagesimalString) {
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

    // -- retrieve additional data if neesscary
    // TODO below call disabled since sequences are not yet supported,
    //      see  #3347 (services and REST service controller for molecular classes implemented)
    //
    // cdm_load_annotations($sequence);

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
                            '#markup' => compose_cdm_media_gallerie(array('medialist' => $value)),
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
 * Crates an array which can be used in render arrays to be passed to the
 * theme_table() and theme_list().
 *
 * @param array $fieldunit_dtos
 *     list of FieldUnitDTOs
 */
function specimen_render_array_items(array $fieldunit_dtos){

  $render_array_items = array();
  $items = array();

  //we need one more item to contain the items of one level (fieldunit, derivate data etc.)
  foreach ($fieldunit_dtos as &$fieldunit_dto) {

    $items['data'] = $fieldunit_dto->listLabel ;
    $specimen = compose_cdm_specimen_or_observation_tree_entry($fieldunit_dto);
    $children = array();
    $child = array();
    $child['data'] =$specimen;
    // $children[] = create_specimen_array($specimenOrObservation->derivates);
    if (isset($fieldunit_dto->derivates) && sizeof($fieldunit_dto->derivates) > 0){
      $child['children']= specimen_render_array_items($fieldunit_dto->derivates);
    }
    $children[]=$child;
    $items['children'] = $children;
    $render_array_items[] = $items;
  }
  return $render_array_items;
}

/**
 * Creates the drupal render array for the table showing all derivatives which
 * stem from a common gathering event.
 *
 * @param $field_unit_uuids array
 *  An array of uuids for cdm FieldUnit entities.
 *
 * @return array
 *  A drupal render array for a table
 *
 * @ingroup compose
 */

function compose_compressed_specimen_derivate_table($field_unit_uuids) {

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

  foreach ($field_unit_uuids as $field_unit_uuid) {

    //get derivate hierarchy for the FieldUnit
    $derivateHierarchy = cdm_ws_get(CDM_WS_PORTAL_OCCURRENCE, array($field_unit_uuid, 'derivateHierarchy'));
    if ($derivateHierarchy) {
      //summary row
      $rows[] = array(
        'data' => array(
          array(
            'data' => $expand_icon . $collapse_icon,
            'class' => array('summary_row_cell', 'summary_row_icon', 'expand_column')
          ),
          array(
            'data' => $derivateHierarchy->country,
            'class' => array('summary_row_cell')
          ),
          array(
            'data' => $derivateHierarchy->date,
            'class' => array('summary_row_cell')
          ),
          array(
            'data' => $derivateHierarchy->collectingString,
            'class' => array('summary_row_cell')
          ),
          @array(
            'data' => $derivateHierarchy->collection,
            'class' => array('summary_row_cell')
          ),
          array(
            'data' => $derivateHierarchy->hasType ? $checked_box_icon : "",
            'class' => array('summary_row_cell', 'summary_row_icon')
          ),
          array(
            'data' => $derivateHierarchy->hasSpecimenScan ? $checked_box_icon : "",
            'class' => array('summary_row_cell', 'summary_row_icon')
          ),
          array(
            'data' => ($derivateHierarchy->hasDna ? $sequence_icon : "") . " "
              . ($derivateHierarchy->hasDetailImage ? $detail_image_icon : "") . " "
              . ($derivateHierarchy->hasCharacterData ? $character_data_icon : ""),
            'class' => array('summary_row_cell', 'summary_row_icon')
          )
        ),
        'id' => 'derivate_summary' . $rowcount, // summary row id
        'class' => array('summary_row'),
      );

      //assemble field unit details
      $detail_html = "";
      // - citation
      if ($derivateHierarchy->citation) {
        $detail_html .= create_label("Citation") . $derivateHierarchy->citation . "<br>";
      }
      //assemble specimen details
      if ($derivateHierarchy->derivates) {
        foreach ($derivateHierarchy->derivates as $preservedSpecimenDTO) {
          $detail_html .= "<br>";
          $detail_html .= render_cdm_specimenDTO_page($preservedSpecimenDTO);
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
 * Composes the view on specimens and occurrences as derivate tree
 * starting from the field unit including all derivatives.
 *
 * @see DM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE
 *
 * @param object $fieldunit_dtos
 *   list of FieldUnitDTOs
 * @return array
 *   The Drupal render array
 *
 * @ingroup compose
 */
function compose_specimen_table_top_down(array $fieldunit_dtos){

  $specimen_render_array_items = specimen_render_array_items($fieldunit_dtos);

    // add icons
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
  $tableId = "specimen_tree_table";
  $specimen_table = array(
    '#theme' => 'table',
    // prefix attributes and rows with '#' to let it pass toF the theme function,
    // otherwise it is handled as child render array
    '#attributes' => array(
      'class' => 'specimens ' . CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE ,
      'id' => $tableId
    ),
    '#rows' => array(),
  );
  $rowcount = 0;
  foreach($specimen_render_array_items as $value){
    $renderArray = array(
      '#theme' => 'item_list',
      '#items' => $value['children'],
      '#type' => 'ul');
    $output = drupal_render($renderArray);

    $specimen_table['#rows'][] = array(
      'data' =>array(
        array(
          'data' => $expand_icon . $collapse_icon,
          'class' => array('summary_row_cell', 'summary_row_icon', 'expand_column')
        ),
        $value['data']
      ),
      'id' => 'label' . $rowcount, // summary row id
      'class' => array('summary_row'),
    );
    $specimen_table['#rows'][] = array(
      'data' =>  array(
        array(
          'data' => "", //empty first column
          'class' => array('expand_column')
        ),
        array(
          'data' => $output

        )
      ),
      'class' =>  array('detail_row')
//                'class' =>  array(
//                    'descriptionElement',
//                    'descriptionElement_IndividualsAssociation'
      //               ),
    );
    $rowcount++;

  }
  drupal_add_js_rowToggle("#".$tableId);

  return $specimen_table;
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

function compose_cdm_specimen_or_observation_tree_entry($specimen_or_observation)
{
    $exclude_occurrence_fields = &drupal_static(__FUNCTION__);
    if (!isset($exclude_occurrence_fields)) {
        $exclude_occurrence_fields = array(
            'type',
            'taxonRelatedDerivedUnits',
            'label',
            'titleCache',
            'listLabel',
            'protectedTitleCache',
            'class',
            'uuid',
            'derivates',
            'collection'
        );
    }
    $items = array();

    // only show uuid it the user is logged in
    if (user_is_logged_in() && ($a_idx = array_search('uuid', $exclude_occurrence_fields)) !== FALSE) {
      //  unset($exclude_occurrence_fields[$a_idx]);
    }


    if (is_object($specimen_or_observation)) {

        $type_label = $specimen_or_observation->recordBase;
        RenderHints::setFootnoteListKey($type_label . '-' . $specimen_or_observation->uuid);

        // collect typeStatus as label
        if (isset($specimen_or_observation->specimenTypeDesignations)) {
            $type_status = array();
            foreach ($specimen_or_observation->specimenTypeDesignations as $typeDesignation) {
                if (isset($typeDesignation->typeStatus->representation_L10n)) {
                    $type_status[] = $typeDesignation->typeStatus->representation_L10n;
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

        $items['data'] = $title;

        $groups = array();
        $items['children'] = $groups;
        $children_items = array();
        // --- add initialized fields
        foreach (get_object_vars($specimen_or_observation) as $field => $value) {
            $child_item = array();

            if (!in_array($field, $exclude_occurrence_fields) && ($value && (!is_object($value) || isset($value->class)))) {
                switch ($field) {

                    /* ---- SpecimenOrObservationBase --- */
                    case 'derivationEvent':
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value, NULL, 1);
                        break;
                    case 'kindOfUnit':
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value, NULL, 1);
                        break;
                    case 'accessionNumber':
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value, NULL, 0);
                        break;

                    case 'preferredStableUri':

                        @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), array(array('#markup' => cdm_external_uri($value, false))));
                        break;

                    case 'specimenTypeDesignations':
                        @_description_list_group_add(
                            $groups,
                            cdm_occurrence_field_name_label($field),
                            array(
                                '#markup' => render_type_designations($value),
                            )
                        );
                        break;



                    case 'listOfMedia':
                        $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME);

                        $captionElements = array(
                            'title',
                            '#uri' => t('open media'),
                        );
                        $gallery_html = compose_cdm_media_gallerie(array(
                            'mediaList' => $value,
                            'galleryName' => $specimen_or_observation->uuid,
                            'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
                            'cols' => $gallery_settings['cdm_dataportal_media_cols'],
                            'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? isset($gallery_settings['cdm_dataportal_media_maxRows']) : null,
                            'captionElements' => $captionElements,
                            'mediaLinkType' => 'LIGHTBOX',
                            'alternativeMediaUri' => NULL,
                            'galleryLinkUri' => NULL,
                            'showCaption' => true
                        ));

                         //@_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $gallery_html);
                         break;



                    /* ---- DerivedUnitBase --- */


                    case 'collectionDTO':

                        $sub_dl_groups = array();
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('code'), $value->code, NULL, 1);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('codeStandard'), $value->codeStandard, NULL, 2);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('institute'), $value->institute, NULL, 3);
                        @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('townOrLocation'), $value->townOrLocation, NULL, 4);
                        // TODO "superCollection"
                        // TODO may have media

                        @_description_list_group_add($groups, 'Collection',
                            array(
                                array('#markup' => $value->titleCache),
                                array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                            )
                        );

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
                    // TODO exsiccatum


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
                            $sub_dl_groups = array();
                            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('errorRadius'), $value->exactLocation->errorRadius, ' m', 1);
                            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('longitude'), round($value->exactLocation->longitude, 7), '°', 2);
                            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('latitude'), round($value->exactLocation->latitude, 7), '°', 3);
                            if (isset($value->exactLocation->referenceSystem)) {
                                @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('referenceSystem'), $value->exactLocation->referenceSystem->representation_L10n, '', 4);
                            }
                            if ( isSet($value->exactLocation->sexagesimalString)){
                                @_description_list_group_add($groups, cdm_occurrence_field_name_label('exactLocation'),
                                    array(
                                        array('#markup' => $value->exactLocation->sexagesimalString),
                                        array(
                                            '#theme' => 'description_list',
                                            '#groups' => $sub_dl_groups
                                        ),
                                    ), '', 11
                                );
                            }
                        }

                        break;

                    /* ---- DerivationEvent --- */
                    case 'derivationEvents':
                        //@_description_list_group_add($groups, t('Association type'), $value->description);
                        break;
                    case 'determinedNames':
                        $dd_elements = array();
                        foreach ($value as $name) {
                            $taxon_name = cdm_ws_get(CDM_WS_NAME, $name->key);
                            $taxon_name->taggedName = cdm_ws_get(CDM_WS_NAME, array($name->key, "taggedName"));
                            $name_link = path_to_name($name->key);

                            //$taxon_html = render_taxon_or_name($taxon_name, $name_link);
                            $taxon_html = l($taxon_name->titleCache, $name_link);
                            $dd_elements[] = $taxon_html;
                        }
                        @_description_list_group_add($groups, cdm_occurrence_field_name_label("Identification(s)"), $dd_elements,'', 100);
                        break;


                    default:
                        if (is_object($value) || is_array($value)) {
                            drupal_set_message("Unhandled type in compose_cdm_specimenOrObservation() for field " . $field, "warning");
                        } else {
                            if ($field == 'recordBase' && $value == 'Dna Sample'){
                                _description_list_group_add($groups, cdm_occurrence_field_name_label($field), 'DNA Sample');
                            }else{
                                _description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value);
                            }
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
            '#attributes' => array('class' => html_class_attribute_ref($specimen_or_observation)),
        );
        $output = drupal_render($occurrence_elements);
        if (isset($gallery_html)){
            $output .= $gallery_html;
        }
        $pathToSpecimen = path_to_specimen($specimen_or_observation->uuid);
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
 * Composes a BOTTOM-UP-SPECIMEN-TABLE
 * @param $specimensOrObservations
 * @return array
 *  A drupal render array with the following keys:
 *   - 'specimen_list'
 *   - 'pager'
 *
 * @ingroup Compose
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
          $type_label .= ': ';
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

        $gallery_html = compose_cdm_media_gallerie(array(
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

        $gallery_html = compose_cdm_media_gallerie(array(
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
 * @param $specimensOrObservations_or_fieldUnitDTO
 *
 * @return array
 */
function order_specimens_or_observations_by_date_and_type($specimensOrObservations)
{
// order occurrences by date but types should be on top of the list
  $type_specimens = array();
  $other_occurrences = array();
  foreach ($specimensOrObservations as &$occurrence) {
    $typeDesignationsPager = cdm_ws_get(CDM_WS_OCCURRENCE . '/$0/specimenTypeDesignations', $occurrence->uuid);
    if (isset($typeDesignationsPager->count) && $typeDesignationsPager->count > 0) {
      $type_specimens[] = $occurrence;
    } else {
      $other_occurrences[] = $occurrence;
    }
  }
  $specimensOrObservations = array_merge($type_specimens, $other_occurrences);
  return $specimensOrObservations;
}

/**
 * Orders FieldUnitDTOs by date but types should be on top of the list.
 *
 * Delegates internally to order_specimens_or_observations_by_date_and_type()
 *
 * @param $fieldUnitDTOs
 *
 * @return array
 */
function order_fieldUnitDtos_by_date_and_type($fieldUnitDTOs)
{
  return order_specimens_or_observations_by_date_and_type($fieldUnitDTOs);
}

