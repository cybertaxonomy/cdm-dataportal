/**
* Copyright (C) 2020 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;

import com.thoughtworks.selenium.SeleniumException;

import eu.etaxonomy.dataportal.ElementUtils;

/**
 * @author a.kohlbecker
 * @since Aug 13, 2020
 */
public class DescriptionList extends BaseElement {

    private Map<String, List<DescriptionElement>> descriptionGroups = new HashMap<>();

    /**
     * @param element A WebElement for a DescriptionList ('{@code<dl>}') DOM object.
     */
    public DescriptionList(WebElement element) {
        super(element);
        if(!element.getTagName().equals("dl")) {
            throw new SeleniumException("Expecting DescriptionList (<dl>) as paramter element, but was " + ElementUtils.webElementTagToMarkup(element));
        }
        List<DescriptionElement> descriptionElements = null;
        for(WebElement we : element.findElements(By.xpath("./child::*"))){
            if(we.getTagName().equals("dt")) {
                descriptionElements = new ArrayList<>();
                descriptionGroups.put(we.getText(), descriptionElements);
            }
            if(we.getTagName().equals("dd")) {
                if(descriptionElements == null) {
                    // oops, this is a dd element without preceding dt
                    descriptionElements = new ArrayList<>();
                    descriptionGroups.put("MISSING DT ELEMENT BEFORE DD", descriptionElements);
                }
                descriptionElements.add(new DescriptionElement(we));
            }
        }
    }

    /**
     *
     * @return the descriptionGroups
     */
    public Map<String, List<DescriptionElement>> getDescriptionGroups() {
        return descriptionGroups;
    }

    /**
     *
     * @param descriptionGroupsKey The text of the {@code <dt>} element preceding
     * one or more {@code <dd>} elements. Text of multiple {@code <dd>} will be joined
     * using the new line ('{@code \n}') character.
     *
     * @return the DescriptionElementTexts joined with '{@code \n}' or null
     * if the key is not present in the map.
     */
    public String joinedDescriptionElementText(String descriptionGroupsKey) {
        String joinedText = null;
        if(descriptionGroups.containsKey((descriptionGroupsKey))) {
            joinedText = descriptionGroups.get(descriptionGroupsKey)
                    .stream()
                    .map(de -> de.getDescriptionElementText())
            .collect(Collectors.joining("\n"));
        }
        return joinedText;
    }

    public static class DescriptionElement extends BaseElement {

        private DescriptionList subList = null;

        DescriptionElement(WebElement descriptionElement){
            super(descriptionElement);
            try {
                subList = new DescriptionList(descriptionElement.findElement(By.tagName("dl")));
            } catch (NoSuchElementException e1) {
                // IGNORE
            }
        }

        /**
         * @return the subList
         */
        public DescriptionList getSubList() {
            return subList;
        }


        public String getDescriptionElementText() {
            return this.getText();
        }
    }

}
