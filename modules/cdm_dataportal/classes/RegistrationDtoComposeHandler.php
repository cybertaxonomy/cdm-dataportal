<?php

class RegistrationDTOComposeHandler extends ItemComposeHandler {

  public function getClassAttributes($item) {
    return $this->classAttributes(new TypedEntityReference("Registration", $item->uuid));
  }

  public function composeItem($item) {
    return compose_registration_dto_compact($item, 'item-style', 'div');
  }
}
