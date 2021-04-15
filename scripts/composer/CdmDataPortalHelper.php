<?php

/**
 * @file
 * Contains \Cybertaxonomy\composer\CdmDataportalHelper.
 */

namespace Cybertaxonomy\composer;

use Composer\Script\Event;
use Composer\Semver\Comparator;
use DrupalFinder\DrupalFinder;
use Symfony\Component\Filesystem\Filesystem;

class CdmDataPortalHelper {

  public static function createPolyfillSymlinks(Event $event) {
    $fs = new Filesystem();
    $drupalFinder = new DrupalFinder();
    $drupalFinder->locateRoot(getcwd());
    $drupalRoot = $drupalFinder->getDrupalRoot();

    // Prepare the settings file for installation
    if (!$fs->exists($drupalRoot . '/polyfills')) {
      $fs->remove($drupalRoot . '/polyfills');
      $event->getIO()->write("Existing ./polyfills removed");
    }
    $fs->symlink('./sites/all/themes/contrib/zen_dataportal/polyfills', $drupalRoot . '/polyfills');
    $event->getIO()->write("Created a symlink from ./polyfills to ./sites/all/themes/contrib/zen_dataportal/polyfills");
  }

  public static function createInstallationPackage(Event $event) {
    $fs = new Filesystem();
    $drupalFinder = new DrupalFinder();
    $drupalFinder->locateRoot(getcwd());
    $drupalRoot = $drupalFinder->getDrupalRoot();

    $parent = dirname($drupalRoot);
    $pd = new PharData('drupal_7_cdm_dataportal.tar');
    $pd->buildFromDirectory($parent, '#^'.preg_quote("$parent/parent-folder/", "#").'#');
    $pd->compress(Phar::GZ);
  }

}
