/**
* Copyright (C) 2021 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.List;
import java.util.stream.Collectors;

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.selenium.XPathTools;

/**
 * @author a.kohlbecker
 * @since Mar 18, 2021
 */
public class DerivedUnitTree extends BaseElement {


    private List<DerivedUnitTreeNode> rootNodes = null;


    public DerivedUnitTree(WebElement element) {
        super(element);
        rootNodes = element.findElements(By.xpath("./div" + XPathTools.classAttrContains("item-list") +"/ul/li")).stream()
        .map(el -> new DerivedUnitTreeNode(el))
        .collect(Collectors.toList());
    }

    public static DerivedUnitTree from(WebElement element) {
        return new DerivedUnitTree(element);
    }

    public List<DerivedUnitTreeNode> getRootNodes() {
        return rootNodes;
    }

}
