/**
* Copyright (C) 2021 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.selenium;

/**
 * @author a.kohlbecker
 * @since Mar 19, 2021
 */
public class XPathTools {

    public static String classAttrContains(String className) {
        return "[contains(concat(' ',normalize-space(@class),' '),' " + className + " ')]";
    }

}
