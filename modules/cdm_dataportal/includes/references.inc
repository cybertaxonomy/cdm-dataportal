<?php

/**
 * @file
 * Functions for dealing with CDM entities from the package model.references
 *
 * @copyright
 *   (C) 2007-2016 EDIT
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
 * Functions which are composing Drupal render arrays
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
 * Cleanup of the textual short form of an author team.
 *
 * The cdm fails to create correct representations of author teams.
 * The '&' gluing the authors together need to be cleaned up.
 *
 * @param $author_team
 *  the authorTeam string
 *
 * @return string

 */
function _short_form_of_author_team($author_team) {
  $number_of_authors = substr_count($author_team, ' & ') + 1;
  // var_dump($author_team);
  // var_dump($number_of_authors);
  switch ($number_of_authors) {
    case 1:
      $result = $author_team;
      break;

    case 2:
      $result = str_replace(' & ', ' and ', $author_team);
      break;

    default:
      $result_parts = explode(' & ', $author_team);
      $result = $result_parts[0] . ' et al.';
  }
  return $result;
}

/**
 * Creates a pager widget for the given CDM Pager containing CDM Reference entities.
 *
 * @param $referencePager object
 *    A CDM Pager  containing CDM Reference entities
 * @param $path
 *    The base path to be used when generating the pager item links
 * @$parameters
 *
 * @return string
 *   Markup
 */
function cdm_reference_pager($referencePager, $path, $parameters) {

  drupal_set_title(t('Bibliographic index'), PASS_THROUGH);
  $out = '';
  if (count($referencePager->records) > 0) {
    $out .= '<ul>';
    foreach ($referencePager->records as $reference) {
      // FIXME remove hack.
      $reference->fullCitation = $reference->titleCache;
      // For matching cdm entity to STO.
      $out .= '<li>' . theme('cdm_reference', array(
          'reference' => $reference,
          'microReference' => NULL,
          'doLink' => TRUE,
        )) . '</li>';
    }
    $out .= '</ul>';
    $out .= theme('cdm_pager', array(
      'pager' => $referencePager,
      'path' => $path,
      'parameters' => $parameters,
    ));
  }
  else {
    $out = '<h4 class="error">Sorry, this page contains not entries.</h4>';
  }
  return $out;
}