<?php
/**
 * PHP implementation of the cdm eu.etaxonomy.cdm.persistence.dao.common.Restriction
 */

class Restriction
{
  public $propertyName;

  public $matchMode;

  public $operator;

  public $values = array();


  /**
   * Public constructor.
   */
  public function __construct($propertyName, $matchMode, array $values, $operator = 'AND') {
    $this->propertyName = $propertyName;
    $this->matchMode = $matchMode;
    $this->values = $values;
    $this->operator = $operator;
  }
}