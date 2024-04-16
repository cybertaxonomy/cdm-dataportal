<?php

/**
 * Class GalleryItemMedia filters suitable MediaRepresentations from a CDM Media
 * object and prepares them for to be used as item in a MediaGallery.
 */
class GalleryItemMedia {

  public static $OK = 0;
  public static $ACCESS_DENIED = -1;
  public static $ERROR = -2;

  public static $image_mime_type_list_default =  [
    'image/jpg',
    'image/jpeg',
    'image/png',
    'image/gif',
  ];

  private $image_mime_type_list;
  private $thumbnail_width;
  private $thumbnail_height;

  private $thumbnail_representation = null;
  private $full_size_representation = null;
  private $web_app_representation = null;
  private $first_representation = null;

  /**
   * GalleryItemMedia constructor.
   */
  public function __construct($media, $thumbnail_width, $thumbnail_height, $image_mime_type_list = null) {
    if(!$image_mime_type_list){
      $this->image_mime_type_list  =  self::$image_mime_type_list_default;
    }
    $this->thumbnail_width = $thumbnail_width;
    $this->thumbnail_height =$thumbnail_height;
    $this->findRepresentations($media);
  }

  private function findRepresentations($media) {

    if (is_array($media->representations) && isset($media->representations[0]->parts[0])) {
      $thumbnail_representations = cdm_preferred_media_representations($media, $this->image_mime_type_list,
          $this->thumbnail_width, $this->thumbnail_height, FALSE, FALSE);
        
      $full_size_representations = cdm_preferred_media_representations($media, $this->image_mime_type_list);
      $web_app_representations = cdm_preferred_media_representations($media, $this->image_mime_type_list, NULL, NULL, TRUE);

      if (isset_not_empty($media->representations)) {
        // can be used as last resort fall back
        $this->first_representation = $media->representations[0];
      }

      if (isset_not_empty($this->thumbnail_representation)) {
        $this->thumbnail_representation = array_shift($thumbnail_representations);
      }
      if (isset_not_empty($full_size_representations)) {
        $this->full_size_representation = array_shift($full_size_representations);
      }
      if (isset_not_empty($web_app_representations)) {
        $this->web_app_representation = array_shift($web_app_representations);
      }
    } else if (isset($media->representations->items[0])) {
          $thumbnail_representations = cdm_preferred_media_representations($media, $this->image_mime_type_list,
              $this->thumbnail_width, $this->thumbnail_height, FALSE, FALSE);

          $full_size_representations = cdm_preferred_media_representations($media, $this->image_mime_type_list);
          $web_app_representations = cdm_preferred_media_representations($media, $this->image_mime_type_list, NULL, NULL, TRUE);

          if (isset_not_empty($media->representations)) {
              // can be used as last resort fall back
              $this->first_representation = $media->representations[0];
          }

          if (isset_not_empty($this->thumbnail_representation)) {
              $this->thumbnail_representation = array_shift($thumbnail_representations);
          }
          if (isset_not_empty($full_size_representations)) {
              $this->full_size_representation = array_shift($full_size_representations);
          }
          if (isset_not_empty($web_app_representations)) {
              $this->web_app_representation = array_shift($web_app_representations);
          }
      }
  }

  public function getThumbnailImageMediaItem() {
    $url = $this->getThumbnailImageUrl();
    if($url){
      return new ImageMediaItem($this->getThumbnailImageUrl(), $this->getThumbnailSize());
    }
    return null;
  }

  /**
   * Provides the size of the thumbnail image if available.
   * In case this GalleryItemMedia contains as web app url the favicon
   * default size is returned.
   *
   * @return object|null
   */
  private function getThumbnailSize() {
    $repr = $this->getThumbnailRepresentation();
    if($repr){
      return $this->sizeOf($repr);
    } else  if($this->hasWebAppItem()) {
      // returning the favicon size, actual size is not important, in this case!
      return new MediaSize(64,64);
    }
    return null;
  }

  /**
   * The url of the thumbnail image if this is available. In case this
   * GalleryItemMedia contains as web app url the according favicon is returned
   * as thumbnail url.
   *
   * @return string|null
   *   The URL
   */
  private function getThumbnailImageUrl() {
    $repr = $this->getThumbnailRepresentation();
    if($repr){
      return $this->urlOf($repr);
    } else if($this->hasWebAppItem()) {
      return $this->webAppIcon($this->getWebAppUrl());
    }
    return null;
  }

  public function getWebAppUrl() {
    return $this->urlOf($this->web_app_representation);
  }

  public function hasWebAppItem(){
    return is_string($this->getWebAppUrl());
  }

  public function getFullSizeImageUrl() {
    if($this->full_size_representation){
      return $this->urlOf($this->full_size_representation);
    }
  }

  public function hasFullSizeImage(){
    return is_string($this->getFullSizeImageUrl());
  }

  /**
   * Provides the ULR to be used for showing the the item
   * in an overlay widget.
   *
   * This is currently limited to images only.
   *
   * @return string
   */
  public function getOverlayImageUrl(){
    if($this->hasFullSizeImage()){
      return $this->getFullSizeImageUrl();
    }
    return null;
  }

  /**
   * Provides the URL to be used for linking directly to the item.
   * Such links will open the item directly in the browser window.
   * WebApp urls are preferred over full-size-image urls.
   * The media item can be a web-app or image.
   *
   * @return mixed
   */
  public function getOpenItemUrl(){

    if($this->hasWebAppItem()){
      $url = $this->getWebAppUrl();
      if($this->urlIsAccessible($url) == self::$OK){
        return $url;
      }
    }
    if($this->hasFullSizeImage()){
      $url = $this->getFullSizeImageUrl();
      if($this->urlIsAccessible($url) == self::$OK){
        return $url;
      }
    }
    return null;
  }

  private function getThumbnailRepresentation() {
    if ($this->thumbnail_representation) {
      return $this->thumbnail_representation;
    } else {
      // use fallback strategy
      if ($this->full_size_representation) {
        return $this->full_size_representation;
      } else if(!$this->web_app_representation->uuid || $this->first_representation->uuid !== $this->web_app_representation->uuid){
        return $this->first_representation;
      }
    }
  }

  /**
   * @param $media_representation
   *  The CDM MediaRepresentation
   *
   * @return string|null
   *  The fist URL found in the representation parts or null
   */
  private function urlOf($media_representation){
    if(isset($media_representation->parts)){
      foreach($media_representation->parts as $part){
        if(isset($part->uri)){
          return $part->uri;
        }
      }
    }
    return null;
  }

  /**
   * @param $url
   *
   * @return false|mixed|string
   *
   * function found at https://stackoverflow.com/questions/5701593/how-to-get-a-websites-favicon-with-php#5702084
   */
  private function webAppIcon($url){
    $favicon = new FaviconDownloader($url);
    if ($favicon->icoExists) {
      return $favicon->icoUrl;
    }
    return null;
  }

  /**
   * @param $url
   *
   * @return int
   *  One of the GalleryItemMedia constants: $OK, $ACCESS_DENIED, $ERROR
   */
  private function urlIsAccessible($url){
    $result = drupal_http_request($url, ['method' => 'HEAD']);
    if($result->code == 200){
      return GalleryItemMedia::$OK;
    } else if($result->code >= 401 && $result->code <= 403) {
      return GalleryItemMedia::$ACCESS_DENIED;
    } else {
      return GalleryItemMedia::$ERROR;
    }
  }

  /**
   * @param $media_representation
   *
   * @return MediaSize object
   *    A MediaSize object with the fields
   *    - width: may be unset
   *    - height: may be unset
   *   or null if no size information is available
   */
  private function sizeOf($media_representation){
    if(isset($media_representation->parts[0])){
      $part = $media_representation->parts[0];
      if (isset_not_empty($part->width) || isset_not_empty($part->height)) {
        return new MediaSize(
          @$media_representation->parts[0]->width,
          @$media_representation->parts[0]->height
        );
      }
    }
    return null;
  }


}