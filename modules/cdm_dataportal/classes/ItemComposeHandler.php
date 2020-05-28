<?php


abstract class ItemComposeHandler {

  abstract public function getClassAttributes($item);

  abstract public function composeItem($item);

  /**
   * @param $cdm_entity
   * A CDM native entity. In case of DTOs the cdm entity can be substituted by
   * TypedEntityReference
   *
   * @return string
   */
  protected function classAttributes($cdm_entity) {
    return html_class_attribute_ref($cdm_entity);
  }
}
