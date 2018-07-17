<?php

/**
 * TypedEntityReference is a class used in the cdmlib. This php impementation of TypedEntityReference
 * can be used to wrap type and uuid into an object suitable to be passed to methods like:
 *  - @see html_class_attribute_ref()
 *
 * User: andreas
 * Date: 16.07.18
 * Time: 18:58
 */

class TypedEntityReference
{
  public $type;
  public $uuid;
  public $class = 'TypedEntityReference';
  /**
   * Private constructor.
   */
  public function __construct($type, $uuid) {
    $this->$type = $type;
    $this->$uuid = $uuid;
  }

}