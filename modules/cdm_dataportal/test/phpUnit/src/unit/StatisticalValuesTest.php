<?php
use PHPUnit\Framework\TestCase;

// these includes require <includePath>../..</includePath> to be set in phpUnit.xml
include 'includes/common.inc';
include 'cdm_api/commons.php';

/**
 * test to test if phpUnit is ok
 * @author a.kohlbecker
 *
 */
class StatisticalValuesTest extends TestCase {

  function new_statistical_value($value = null){
    $stat_val = new stdClass();
    $stat_val->_value = $value;
    return $stat_val;
  }

  function create_statistical_values($typicalLowerBoundary = null, $typicalUpperrBoundary = null, $average  = null, $sampleSize = null){
    $stat_vals = statistical_values_array();
    $stat_vals['TypicalLowerBoundary'] = $this->new_statistical_value($typicalLowerBoundary);
    $stat_vals['TypicalUpperBoundary'] = $this->new_statistical_value($typicalUpperrBoundary);
    $stat_vals['SampleSize'] = $this->new_statistical_value($sampleSize);
    $stat_vals['Average'] = $this->new_statistical_value($average);
    return $stat_vals;
  }

  function html2text($html){
    return  html_entity_decode(strip_tags($html), ENT_COMPAT, 'utf-8');
  }

  function test_statistical_values() {

    $stat_vals = $this->create_statistical_values(0.123457,  0.123456, 0.123456523847, 5);

    $this->assertEquals('0.123457–0.123456[5;x̄=0.1234565]', $this->html2text(statistical_values($stat_vals)));
  }

}
