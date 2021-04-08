<?php
/**
 * @file
 * Functions und constants providing symbols
 *
 * @see http://www.mozilla.org/MPL/MPL-1.1.html
 *
 * @copyright
 *   (C) 2007-2021 EDIT
 *   European Distributed Institute of Taxonomy
 *   http://www.e-taxonomy.eu
 *
 *   The contents of this module are subject to the Mozilla
 *   Public License Version 1.1.
 * @author
 *   - Andreas Kohlbecker <a.kohlbecker@BGBM.org>
 */

const SYMBOL_COLLAPSIBLE_CLOSED = 'chevron-right';
const SYMBOL_COLLAPSIBLE_OPEN = 'chevron-down';

/**
 * @param $base_of_record_uuid
 *  The UUID of the SpecimenOrObservationType
 *
 * @return String
 *  Markup for a font awesome icon
 */
function symbol_for_base_of_record($base_of_record_uuid, $attributes = array()){

  switch($base_of_record_uuid){
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_PRESERVEDSPECIMEN :
      $fa_icon_name = 'leaf'; // TODO 'pagelines'
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_FOSSIL :
      $fa_icon_name = 'bone';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_LIVING_SPECIMEN :
      $fa_icon_name = 'tree';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_OBSERVATION :
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_MACHINE_OBSERVATION :
      $fa_icon_name = 'binoculars';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_HUMAN_OBSERVATION :
      $fa_icon_name = 'eye';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_TISSUE_SAMPLE :
      $fa_icon_name = 'cut';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_DNA_SAMPLE :
      $fa_icon_name = 'dna';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_STILL_IMAGE :
      $fa_icon_name = 'image';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_MOVING_IMAGE :
      $fa_icon_name = 'film';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_SOUND_RECORDING :
      $fa_icon_name = 'microphone';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_MEDIA :
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_MULTIMEDIA :
      $fa_icon_name = 'photo-video';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_FIELD_UNIT :
      $fa_icon_name = 'mountain';
      break;
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_MATERIAL_SAMPLE :
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_UNKNOWN :
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_OTHER_SPECIMEN :
    case UUID_SPECIMEN_OR_OBSERVATION_TYPE_DERIVEDUNIT :
    default:
      $fa_icon_name = 'cube'; // or 'archive'
  }

  return font_awesome_icon_markup('fa-'.$fa_icon_name, $attributes);
}