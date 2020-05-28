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

  $render_array = [];
  $name_caches = $agent->titleCache;


  $name_details = [];
  if($agent->familyName){
    $name_details[] = '<span class="label">' . t('Family name') . ': </span>'. $agent->familyName;
  }
  if($agent->givenName){
    $name_details[] = '<span class="label">' . t('Given name') . ': </span>'. $agent->givenName;
  }
  $name_details_markup = ' (' .  join(', ', $name_details) .')';
  if($agent->nomenclaturalTitle != $agent->titleCache){
    $name_details_markup .=  ' ' . $agent->nomenclaturalTitle;
  }

  $details[] = $name_details_markup;
  if($agent->lifespan){
    // as first element after the titleCache
    $lifespan_text = timePeriodToString($agent->lifespan);
    if($lifespan_text){
      array_unshift($details, ", " . $lifespan_text);
    }
  }
  $visible_extensions_sorted = visible_extensions_sorted($agent);
  // FIXME:-------------------------------
  // 7d1a269b-9bdd-44eb-beba-2f8acee941af is the phycobank specific ext type for "Life span" which will become obsolte!!!!
  if(isset($visible_extensions_sorted['7d1a269b-9bdd-44eb-beba-2f8acee941af'])){
    // as first element after the titleCache
    array_unshift($details, ", " . $visible_extensions_sorted['7d1a269b-9bdd-44eb-beba-2f8acee941af'][0]->value);
    unset($visible_extensions_sorted['7d1a269b-9bdd-44eb-beba-2f8acee941af']);
  }
  // ------------------------------------
  // extensions
  $extensions = [];
  foreach ($visible_extensions_sorted as $type_uuid => $exts){
    foreach ($exts as $ext){
      if($ext->value){
        $extensions[] = '<span class="label">' . $ext->type->representation_L10n . ': </span>'. $ext->value;
      }
    }
  }
  $extensions_markup = join(', ', $extensions);
  if($extensions_markup){
    $details[] = $extensions_markup;
  }

  // IDs as last
  if(isset($agent->orcid)){
    $corcid_id = number_format($agent->orcid->digitsOnly, 0, '.', '-');
    $details[] = l('https://orcid.org/' . $corcid_id, 'https://orcid.org/' . $corcid_id);
  }
  if($agent->identifiers){
    $identifiers_render_array = compose_identifiers($agent->identifiers);
    $details[] = drupal_render($identifiers_render_array);
  }

  $details_markup = join(' ', $details);

  $render_array = markup_to_render_array($name_caches . $details_markup, null, '<' . $enclosing_tag . ' class="' . html_class_attribute_ref($agent) . '">', '</' . $enclosing_tag . '>');

  return $render_array;
}
