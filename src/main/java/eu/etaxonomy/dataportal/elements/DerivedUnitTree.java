/**
* Copyright (C) 2021 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

/**
 * @author a.kohlbecker
 * @since Mar 18, 2021
 */
public class DerivedUnitTree extends BaseElement {


    private DerivedUnitTreeNode rootNode = null;


    public DerivedUnitTree(WebElement element) {
        super(element);
        rootNode = new DerivedUnitTreeNode(element.findElement(By.cssSelector(".derived-unit-tree-root")));
    }

    public static DerivedUnitTree from(WebElement element) {
        return new DerivedUnitTree(element);
    }



    public DerivedUnitTreeNode getRootNode() {
        return rootNode;
    }

}
