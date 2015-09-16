<?php

/**
 * @file
 * Hooks provided by the CDM API module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define cdm specific node types.
 *
 * Modules implementing this hook can add cdm specific node types
 *
 */
function hook_cdm_nodetypes() {
  static $nodetypes;
  if (!$nodetypes) {
    $nodetypes = array(
        'cdm_treference' => 'reference',
        'cdm_taxon' => 'taxon',
    );
  }
  return $nodetypes;
}

/**
 * @} End of "addtogroup hooks".
 */