<?php


class ImageMediaItem {

  private $size;
  private $url;

  /**
   * ImageMediaItem constructor.
   */
  public function __construct($url, MediaSize $size = null) {
    $this->size = $size;
    $this->url = $url;
  }

  public static function fromMediaRepresentation($mediaRepresentation){
    $h = @$mediaRepresentation->height;
    $w = @$mediaRepresentation->width;
    return new ImageMediaItem($mediaRepresentation->uri, new MediaSize($w, $h));
  }

  /**
   * @return \MediaSize
   */
  public function getSize() {
    return $this->size;
  }

  /**
   * @return mixed
   */
  public function getUrl() {
    return $this->url;
  }

}