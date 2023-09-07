<?php

/**
 * Compose a description as a table of Feature<->State
 *
 * @param $description_uuid
 *
 * @return array
 *    The drupal render array for the page
 *
 * @ingroup compose
 */
function compose_agent($agent, $enclosing_tag) {


  $names_and_lifespan = $agent->titleCache;

  if(isset($agent->nomenclaturalTitle) && $agent->nomenclaturalTitle != $agent->titleCache){
    $names_and_lifespan .=  ' [' . $agent->nomenclaturalTitle . ']';
  }

  $visible_extensions_sorted = visible_extensions_sorted($agent);
  // ------------------------------------
  if(isset($agent->lifespan)){
    $names_and_lifespan .= ', ' . timePeriodToString($agent->lifespan);
  }
  $names_and_lifespan_markup = '<div class="names-and-lifespan">' . $names_and_lifespan . '</div> ';

  $name_details = [];
  $name_details_markup = '';
  if(isset($agent->familyName) || isset($agent->givenName)){
    if(isset($agent->familyName)){
      $name_details[] = '<span class="label">' . t('Family name') . ': </span>'. $agent->familyName;
    }
    if(isset($agent->givenName)){
      $name_details[] = '<span class="label">' . t('Given name') . ': </span>'. $agent->givenName;
    }
    $name_details_markup = ' <div class="name-details">(' .  join(', ', $name_details) .')</div> ';
  }

  // extensions
  $extensions = [];
  foreach ($visible_extensions_sorted as $type_uuid => $exts){
    foreach ($exts as $ext){
      if($ext->value){
        $extensions[] = '<span class="label">' . $ext->type->representation_L10n . ': </span>'. $ext->value;
      }
    }
  }
  $extensions_markup = '';
  if(count($extensions) > 0 ){
    $extensions_markup = '<div class="extensions">' . join(', ', $extensions) . '</div>';
  }

  // IDs
  $identifiers_markup = '';
  $identifiers_render_array = [];
  if(isset($agent->orcid)){
    $pos = 4;
    $orcid_id = substr($agent->orcid->digitsOnly, 0, $pos) ."-".substr($agent->orcid->digitsOnly, 4, $pos) ."-".substr($agent->orcid->digitsOnly, 8, $pos)."-".substr($agent->orcid->digitsOnly, 12, $pos) ;
        //($agent->orcid->digitsOnly, 0, '.', '-');
    $identifiers_render_array[] = markup_to_render_array(l('https://orcid.org/' . $orcid_id, 'https://orcid.org/' . $orcid_id) . ' ');
  }
  if(isset($agent->identifiers)){
    $identifiers_render_array = array_merge($identifiers_render_array, compose_identifiers($agent->identifiers));
  }
  if(count($identifiers_render_array) > 0){
    $identifiers_markup = '<div class="identifier">' . drupal_render($identifiers_render_array) . '</div>';
  }


  $render_array = markup_to_render_array($names_and_lifespan_markup . $name_details_markup  . $extensions_markup . $identifiers_markup, null, '<' . $enclosing_tag . ' class="' . html_class_attribute_ref($agent) . '">', '</' . $enclosing_tag . '>');

  return $render_array;
}
