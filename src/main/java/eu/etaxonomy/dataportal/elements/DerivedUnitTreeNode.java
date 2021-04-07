/**
* Copyright (C) 2021 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.selenium.XPathTools;

/**
 * @author a.kohlbecker
 * @since Mar 18, 2021
 */
public class DerivedUnitTreeNode extends BaseElement {

    private List<DerivedUnitTreeNode> subNodes = new ArrayList<>();
    private WebElement itemWrapper;

    public DerivedUnitTreeNode(WebElement element) {
        super(element);
        // > .item-list > ul.derived-unit-sub-tree > li.derived-unit-item
        String xpathStr =
                "./div" + XPathTools.classAttrContains("item-list")
                + "/ul" + XPathTools.classAttrContains("derived-unit-item")
                + "/li" + XPathTools.classAttrContains("derived-unit-item");
        subNodes = element.findElements(By.xpath(xpathStr))
        .stream()
       // .peek(el -> System.out.println("#####> " + element.getText() + " >>> " + el.getText()))
        .map(el -> new DerivedUnitTreeNode(el))
        .collect(Collectors.toList());
        itemWrapper = element.findElement(By.xpath("./div" + XPathTools.classAttrContains("item-wrapper")));
    }

    public BaseElement getHeader() {
        return BaseElement.from(itemWrapper.findElement(By.cssSelector(".unit-header")));
    }

    public WebElement getTreeNodeSymbol() {
        return itemWrapper.findElement(By.cssSelector(".unit-header .tree-node-symbol"));
    }

    public BaseElement getContent() {
        return BaseElement.from(itemWrapper.findElement(By.cssSelector(".unit-content")));
    }

    public DetailsTable getDetailsTable(String tableClassAttribute) {
        return DetailsTable.from(getContent().getElement().findElement(By.cssSelector("table." + tableClassAttribute)));
    }

    public String getDerivationEvent() {
        return getContent().getElement().findElement(By.cssSelector("div.derivation-event")).getText();
    }


    public List<DerivedUnitTreeNode> getSubNodes() {
        return subNodes;
    }


}
