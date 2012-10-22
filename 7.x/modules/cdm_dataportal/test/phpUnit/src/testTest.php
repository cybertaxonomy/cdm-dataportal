<?php

/**
 * test to test if phpUnit is ok
 * @author a.kohlbecker
 *
 */
class TestTests extends PHPUnit_Framework_TestCase {
  function test_fancymodule() {
    $this->assertInternalType("string", "true");
  }
}
