<?php

/**
 * Truncates a $string to the specified $length.
 * If the supplied string is equal to or shorter than the $legth the original is returend.
 * if an $appendix is defined the resulting string will have the specified $length including the $appendix.
 *
 * @param String $string  the string to truncate
 * @param Number $length  the maximun length
 * @param String $appendix  an optional appendix.
 *
 * @return the string
 */
function str_trunk(&$string, $length, $appendix = '') {
  if (strlen($string) >= $length) {
    return substr($string, 0, $length - strlen($appendix)) . $appendix;
  }
  else {
    return $string;
  }
}

/**
 *
 * @param string $str
 * @param string $sub
 * @return boolean
 */
function str_beginsWith($str, $sub) {
  return (substr($str, 0, strlen($sub)) === $sub);
}

/**
 *
 * @param string $str
 * @param string $sub
 * @return boolean
 */
function str_endsWith($str, $sub) {
  return (substr($str, strlen($str) - strlen($sub)) === $sub);
}

/**
 * Replaces all occurences in $array of the key defined in $replace_map with the
 * according values in the
 * $replace_map.
 *
 * @param
 *          $array
 * @param
 *          $replace_map
 * @return unknown_type
 */
function array_replace_key($array, $replace_map) {
  foreach ($replace_map as $key => $newkey) {
    if (isset($array[$key])) {
      $array[$newkey] = $array[$key];
      unset($array[$key]);
    }
  }
  return $array;
}

/**
 * Replaces all occurrences of space characters with
 * an underscore and transforms the given
 * string to lowercase.
 *
 * @param String $string
 * @return the transformed string
 */
function generalizeString($string) {
  return str_replace(' ', '_', strtolower($string));
}
