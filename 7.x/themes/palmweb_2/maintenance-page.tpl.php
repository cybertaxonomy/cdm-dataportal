<?php
/**
 * @file
 * Custom theme implementation to display a single Drupal page while offline.
 *
 * This file controls the page that is displayed when the site is in
 * "maintenance mode" but the database connection and database are still
 * functioning correctly.
 *
 * @author W. Addink <w.addink@eti.uva.nl>
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>">
  <div id="page">
    <div id="header">
    </div> <!-- /header -->
    <div id="content">
      <?php if (!empty($title)): ?><h1 class="title clear-fix" id="page-title"><?php print $title; ?></h1><?php endif; ?>
      <?php if (!empty($messages)): print $messages; endif; ?>
      <div id="content-content" class="clearfix">
        <?php print $content; ?>
      </div> <!-- /content-content -->
    </div> <!-- /content -->
  </div><!-- /page -->
</body>
</html>
