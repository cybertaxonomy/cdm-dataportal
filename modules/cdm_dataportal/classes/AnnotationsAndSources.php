<?php

/**
 * Class AnnotationsAndSources
 *   - foot_note_keys: array of FootnoteKey objects
 *   - source_references: an array of the source references citations
 *   - names used in source: an associative array of the names in source,
 *        the name in source strings are de-duplicated
 *        !!!NOTE!!!!: this field will most probably be removed soon (TODO)
 */
class AnnotationsAndSources {

  /**
   * @var array of FootnoteKey objects
   */
  private $footNoteKeys = [];

  /**
   * @var array an array of the source references citations
   */
  private $sourceReferences = [];

  /**
   * @var array an associative array of the names in source,
   *        the name in source strings are de-duplicated
   *        !!!NOTE!!!!: this field will most probably be removed soon (TODO)
   */
  private $nameUsedInSource = [];

  /**
   * Pipes the $this->footNoteKeyData) through the render_footnote_keys() function and
   * returns the markup
   *
   * @return string the markup for all $this->footNoteKeyData)
   */
  public function footNoteKeysMarkup() {
    return render_footnote_keys($this->footNoteKeys, ',');
  }

  /**
   * @return array
   */
  public function getFootNoteKeys() {
    return $this->footNoteKeys;
  }

  /**
   * @param array $footNoteKeys
   *  Array of FootnoteKey objects
   */
  public function setFootNoteKeys( array $footNoteKeys) {
    $this->footNoteKeys = $footNoteKeys;
  }

  public function hasFootnoteKeys(){
    return count($this->footNoteKeys) > 0;
  }

  /**
   * @param FootnoteKey $footNoteKey
   *  The FootnoteKey object to add.
   */
  public function addFootNoteKey(FootnoteKey $footNoteKey) {
    $this->footNoteKeys[] = $footNoteKey;
  }

  /**
   * Add all FootnoteKey objects
   *
   * @param $footNoteKeys
   *  The FootnoteKey objects to add.
   */
  public function addAllFootNoteKeys(array $footNoteKeys) {
    foreach($footNoteKeys as $key){
      $this->footNoteKeys[] = $key;
    }
  }

  /**
   * @return array
   */
  public function getSourceReferences() {
    return $this->sourceReferences;
  }

  public function hasSourceReferences() {
    return count($this->sourceReferences) > 0;
  }

  /**
   * Adds a citation string to the
   * @param array $sourceReferenceCitation
   */
  public function addSourceReferencesCitation($sourceReferenceCitation) {
    $this->sourceReferences[] = $sourceReferenceCitation;
  }

  /**
   * @return array
   */
  public function getNameUsedInSource() {
    return $this->nameUsedInSource;
  }

  public function hasNameUsedInSource() {
    return count($this->nameUsedInSource) > 0;
  }

  public function putNamesUsedInSource($plaintext_key, $markup) {
    $this->nameUsedInSource[$plaintext_key] = $markup;
  }



}