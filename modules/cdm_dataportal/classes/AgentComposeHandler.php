<?php

class AgentComposeHandler extends ItemComposeHandler {

  private $enclosing_tag = 'div';

  public function getClassAttributes($item) {
    return $this->classAttributes($item);
  }

  public function composeItem($item) {
    return compose_agent($item, $this->enclosing_tag);
  }
}
