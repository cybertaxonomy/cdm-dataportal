<?php
  /**
   * @file
   * enumerations as defined in the CDM library.
   *
   * @copyright
   *   (C) 2007-2015 EDIT
   *   European Distributed Institute of Taxonomy
   *   http://www.e-taxonomy.eu
   *
   *   The contents of this module are subject to the Mozilla
   *   Public License Version 1.1.
   * @see http://www.mozilla.org/MPL/MPL-1.1.html
   *
   * @author
   *   - Andreas Kohlbecker <a.kohlbecker@BGBM.org>
   */
  abstract class OriginalSourceType {
    const Aggregation = 'Aggregation';
    const Import = 'Import';
    const Lineage = 'Lineage';
    const Other = 'Other';
    const PrimaryMediaSource = 'PrimaryMediaSource';
    const PrimaryTaxonomicSource = 'PrimaryTaxonomicSource';
    const Transformation = 'Transformation';
    const Unknown = 'Unknown';
  }