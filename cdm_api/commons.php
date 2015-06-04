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


function roman_numerals($input_arabic_numeral='') {

  if ($input_arabic_numeral == '') { $input_arabic_numeral = date("Y"); } // DEFAULT OUTPUT: THIS YEAR
  $arabic_numeral            = intval($input_arabic_numeral);
  $arabic_numeral_text    = "$arabic_numeral";
  $arabic_numeral_length    = strlen($arabic_numeral_text);

  if (!preg_match('/[0-9]/', $arabic_numeral_text)) {
    return false; }

  if ($arabic_numeral > 4999) {
    return false; }

  if ($arabic_numeral < 1) {
    return false; }

  if ($arabic_numeral_length > 4) {
    return false; }

  $roman_numeral_units    = $roman_numeral_tens        = $roman_numeral_hundreds        = $roman_numeral_thousands        = array();
  $roman_numeral_units[0]    = $roman_numeral_tens[0]    = $roman_numeral_hundreds[0]    = $roman_numeral_thousands[0]    = ''; // NO ZEROS IN ROMAN NUMERALS

  $roman_numeral_units[1]='I';
  $roman_numeral_units[2]='II';
  $roman_numeral_units[3]='III';
  $roman_numeral_units[4]='IV';
  $roman_numeral_units[5]='V';
  $roman_numeral_units[6]='VI';
  $roman_numeral_units[7]='VII';
  $roman_numeral_units[8]='VIII';
  $roman_numeral_units[9]='IX';

  $roman_numeral_tens[1]='X';
  $roman_numeral_tens[2]='XX';
  $roman_numeral_tens[3]='XXX';
  $roman_numeral_tens[4]='XL';
  $roman_numeral_tens[5]='L';
  $roman_numeral_tens[6]='LX';
  $roman_numeral_tens[7]='LXX';
  $roman_numeral_tens[8]='LXXX';
  $roman_numeral_tens[9]='XC';

  $roman_numeral_hundreds[1]='C';
  $roman_numeral_hundreds[2]='CC';
  $roman_numeral_hundreds[3]='CCC';
  $roman_numeral_hundreds[4]='CD';
  $roman_numeral_hundreds[5]='D';
  $roman_numeral_hundreds[6]='DC';
  $roman_numeral_hundreds[7]='DCC';
  $roman_numeral_hundreds[8]='DCCC';
  $roman_numeral_hundreds[9]='CM';

  $roman_numeral_thousands[1]='M';
  $roman_numeral_thousands[2]='MM';
  $roman_numeral_thousands[3]='MMM';
  $roman_numeral_thousands[4]='MMMM';

  if ($arabic_numeral_length == 3) { $arabic_numeral_text = "0" . $arabic_numeral_text; }
  if ($arabic_numeral_length == 2) { $arabic_numeral_text = "00" . $arabic_numeral_text; }
  if ($arabic_numeral_length == 1) { $arabic_numeral_text = "000" . $arabic_numeral_text; }

  $anu = substr($arabic_numeral_text, 3, 1);
  $anx = substr($arabic_numeral_text, 2, 1);
  $anc = substr($arabic_numeral_text, 1, 1);
  $anm = substr($arabic_numeral_text, 0, 1);

  $roman_numeral_text = $roman_numeral_thousands[$anm] . $roman_numeral_hundreds[$anc] . $roman_numeral_tens[$anx] . $roman_numeral_units[$anu];
  return ($roman_numeral_text);
}

/**
 * Converts an integer into the alphabet base (A-Z).
 *
 * @param int $n This is the number to convert.
 * @return string The converted number.
 * @author Theriault
 *
 */
function num2alpha($n) {
  $r = '';
  for ($i = 1; $n >= 0 && $i < 10; $i++) {
    $r = chr(0x41 + ($n % pow(26, $i) / pow(26, $i - 1))) . $r;
    $n -= pow(26, $i);
  }
  return $r;
}
?>