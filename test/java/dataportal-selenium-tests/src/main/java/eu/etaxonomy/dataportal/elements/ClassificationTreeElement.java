// $Id$
/**
* Copyright (C) 2012 EDIT
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
 * @author andreas
 * @date May 21, 2012
 *
 */
public class ClassificationTreeElement extends BaseElement {


    private String taxonName;

    private String linkUrl;


    private boolean isFocused;

    public String getTaxonName() {
        return taxonName;
    }

    public String getLinkUrl() {
        return linkUrl;
    }

    public boolean isFocused() {
        return isFocused;
    }

    public ClassificationTreeElement(WebElement element) {
        super(element);
        taxonName = element.getText();
        for(String classAttribute : getClassAttributes()){
            if(classAttribute.equals("focused")){
                isFocused = true;
                break;
            }
        }
        linkUrl = element.findElement(By.tagName("a")).getAttribute("href");
    }






}
