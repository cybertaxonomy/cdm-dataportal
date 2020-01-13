<?php

// using namespace to allow for better sorting of test classes in the results
namespace test\phpunit\unit;


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
    $stat_val = new \stdClass(); // need to use the root namespace
    $stat_val->_value = $value;
    return $stat_val;
  }

  function create_statistical_values($typicalLowerBoundary = null, $typicalUpperrBoundary = null, $average  = null, $sampleSize = null, $variance = null, $standard_deviation = null){
    $stat_vals = statistical_values_array();
    $stat_vals['TypicalLowerBoundary'] = $this->new_statistical_value($typicalLowerBoundary);
    $stat_vals['TypicalUpperBoundary'] = $this->new_statistical_value($typicalUpperrBoundary);
    $stat_vals['SampleSize'] = $this->new_statistical_value($sampleSize);
    $stat_vals['Average'] = $this->new_statistical_value($average);
    if($variance){
      $stat_vals['Variance'] = $this->new_statistical_value($variance);
    }
    if($standard_deviation){
      $stat_vals['StandardDeviation'] = $this->new_statistical_value($standard_deviation);
    }
    return $stat_vals;
  }

  function html2text($html){
    return  html_entity_decode(strip_tags($html), ENT_COMPAT, 'utf-8');
  }

  function test_statistical_values_significant_figures_avarage() {

    $stat_vals = $this->create_statistical_values(0.123457,  0.123456, 0.1234565345, 5);
    $this->assertEquals('0.123457–0.123456[5;x̄=0.1234565]', $this->html2text(statistical_values($stat_vals)));

    $stat_vals = $this->create_statistical_values(12.23,  14.2, 13.2231423, 15);
    $this->assertEquals('12.23–14.2[15;x̄=13.2]', $this->html2text(statistical_values($stat_vals)));

    $stat_vals = $this->create_statistical_values(4,  6, 5, 2);
    $this->assertEquals('4–6[2;x̄=5]', $this->html2text(statistical_values($stat_vals)));

    $stat_vals = $this->create_statistical_values(4,  6, 5.0000, 2);
    $this->assertEquals('4–6[2;x̄=5]', $this->html2text(statistical_values($stat_vals)));

    $stat_vals = $this->create_statistical_values(6.3467,  6.3482, 6.347034915, 20);
    $this->assertEquals('6.3467–6.3482[20;x̄=6.34703]', $this->html2text(statistical_values($stat_vals)));

    $stat_vals = $this->create_statistical_values(0.00000001,  1, (1 - 0.00000001) / 2, 20);
    $this->assertEquals('1.0E-8–1[20;x̄=0.5]', $this->html2text(statistical_values($stat_vals)));

    $stat_vals = $this->create_statistical_values(22,  23, 22.5, 4);
    $this->assertEquals('22–23[4;x̄=22.5]', $this->html2text(statistical_values($stat_vals)));

    // see https://dev.e-taxonomy.eu/redmine/issues/8771#note-17
    $stat_vals = $this->create_statistical_values(22,  23, 22.047619, 24);
    $this->assertEquals('22–23[24;x̄=22]', $this->html2text(statistical_values($stat_vals)));
  }


  function test_statistical_values_no_min_max() {

    $stat_vals = $this->create_statistical_values(null,  null, 0.1234, 5);
    $this->assertEquals('0.1234[5]', $this->html2text(statistical_values($stat_vals)));

    // as discussed in https://dev.e-taxonomy.eu/redmine/issues/8771#note-16 variance etc should not be suppressed
    $stat_vals = $this->create_statistical_values(null,  null, 0.1234, 5, 0.3, 0.12);
    $this->assertEquals('0.1234[5;σ²=0.3;σ=0.12]', $this->html2text(statistical_values($stat_vals)));
  }

}
