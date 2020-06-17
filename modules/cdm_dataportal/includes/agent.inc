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

  if($agent->nomenclaturalTitle != $agent->titleCache){
    $names_and_lifespan .=  ' [' . $agent->nomenclaturalTitle . ']';
  }

  $visible_extensions_sorted = visible_extensions_sorted($agent);
  // FIXME:-------------- "Life span" as Extension -------------
  $lifespan_extension = null;
  // 7d1a269b-9bdd-44eb-beba-2f8acee941af is the phycobank specific ext type for "Life span" which will become obsolete!!!!
  if(isset($visible_extensions_sorted['7d1a269b-9bdd-44eb-beba-2f8acee941af'])){
    // as first element after the titleCache
    $lifespan_extension = $visible_extensions_sorted['7d1a269b-9bdd-44eb-beba-2f8acee941af'][0]->value;
    unset($visible_extensions_sorted['7d1a269b-9bdd-44eb-beba-2f8acee941af']);
  }
  // ------------------------------------
  if(!$lifespan_extension && $agent->lifespan){
    $names_and_lifespan .= ', ' . timePeriodToString($agent->lifespan);
  } else {
    $names_and_lifespan .= ", " . $lifespan_extension;
  }

  $names_and_lifespan_markup = '<div class="names-and-lifespan">' . $names_and_lifespan . '</div> ';

  $name_details = [];
  if($agent->familyName){
    $name_details[] = '<span class="label">' . t('Family name') . ': </span>'. $agent->familyName;
  }
  if($agent->givenName){
    $name_details[] = '<span class="label">' . t('Given name') . ': </span>'. $agent->givenName;
  }
  $name_details_markup = ' <div class="name-details">(' .  join(', ', $name_details) .')</div> ';

  // extensions
  $extensions = [];
  foreach ($visible_extensions_sorted as $type_uuid => $exts){
    foreach ($exts as $ext){
      if($ext->value){
        $extensions[] = '<span class="label">' . $ext->type->representation_L10n . ': </span>'. $ext->value;
      }
    }
  }
  if(count($extensions) > 0 ){
    $extensions_markup = '<div class="extensions">' . join(', ', $extensions) . '</div>';
  }

  // IDs
  $identifiers_markup = '';
  $identifiers_render_array = [];
  if(isset($agent->orcid)){
    $orcid_id = number_format($agent->orcid->digitsOnly, 0, '.', '-');
    $identifiers_render_array[] = markup_to_render_array(l('https://orcid.org/' . $orcid_id, 'https://orcid.org/' . $orcid_id) . ' ');
  }
  if($agent->identifiers){
    $identifiers_render_array = array_merge($identifiers_render_array, compose_identifiers($agent->identifiers));
  }
  if(count($identifiers_render_array) > 0){
    $identifiers_markup = '<div class="identifier">' . drupal_render($identifiers_render_array) . '</div>';
  }


  $render_array = markup_to_render_array($names_and_lifespan_markup . $name_details_markup  . $extensions_markup . $identifiers_markup, null, '<' . $enclosing_tag . ' class="' . html_class_attribute_ref($agent) . '">', '</' . $enclosing_tag . '>');

  return $render_array;
}
