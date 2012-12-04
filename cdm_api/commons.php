<?php
/**
 * @file
 * String functions.
 */

/**
 * Truncates a $string to the specified $length.
 *
 * If the supplied string is equal to or shorter than the $length, the original
 * is returned. If an $appendix is defined, the resulting string will have the
 * specified $length including the $appendix.
 *
 * @param string $string
 *   The string to truncate.
 * @param int $length
 *   The maximun length.
 * @param string $appendix
 *   An optional appendix.
 *
 * @return string
 *   The truncated string.
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
 * Checks if a string starts with some substring.
 *
 * @param string $str
 *   The string to test.
 * @param string $sub
 *   The substring that the string should start with.
 *
 * @return bool
 *   TRUE if the string starts with the substring.
 */
function str_beginsWith($str, $sub) {
  return (substr($str, 0, strlen($sub)) === $sub);
}

/**
 * Checks if a string ends with some substring.
 *
 * @param string $str
 *   The string to test.
 * @param string $sub
 *   The substring that the string should end with.
 *
 * @return bool
 *   TRUE if the string ends with the substring.
 */
function str_endsWith($str, $sub) {
  return (substr($str, strlen($str) - strlen($sub)) === $sub);
}

/**
 * Replaces all keys in an array with the given values.
 *
 * All occurences in $array of the key defined in $replace_map are replaced
 * with the according values in the $replace_map.
 *
 * @param array $array
 *   The array to modify.
 * @param array $replace_map
 *   The values to replace.
 *
 * @return array
 *   The modified array.
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
 * Replaces spaces in a string to underscores and returns the string lowercase.
 *
 * All occurrences of space characters are replaced with an underscore.
 * The string is also transformed to lowercase.
 *
 * @param string $string
 *   The string to modify.
 *
 * @return string
 *   The transformed string.
 */
function generalizeString($string) {
  return str_replace(' ', '_', strtolower($string));
}
