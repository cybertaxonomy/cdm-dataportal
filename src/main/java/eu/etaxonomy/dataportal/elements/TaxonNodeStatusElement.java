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
import java.util.List;

import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;

/**
 * @author a.kohlbecker
 */
public class TaxonNodeStatusElement extends BaseElement {

    List<TaxonNodeStatusData> taxonNodeStatus = new ArrayList<>();


    public TaxonNodeStatusElement(WebElement element) {
        super(element);

        List<WebElement> taxonNodeElements = element.findElements(By.className("cdm\\:TaxonNodeDto"));
        for(WebElement el : taxonNodeElements) {
            TaxonNodeStatusData data = new TaxonNodeStatusData();
            data.setTaxonNodeRef(EntityReference.from(el));
            String statusText = el.getText();
            String classificationText = "";
            try {
                WebElement classficationEl = el.findElement(By.className("cdm\\:Classification"));
                classificationText = classficationEl.getText();
                statusText = statusText.replace(classificationText, "");
                data.setClassficationText(classificationText);
                data.setClassificationRef(EntityReference.from(classficationEl));
            } catch (NoSuchElementException e) {
                // IGNORE (classification information is not mandatory) //
            }
            data.setStatusText(statusText);
            taxonNodeStatus.add(data);
        }
    }

    public List<TaxonNodeStatusData> getTaxonNodeStatus() {
        return taxonNodeStatus;
    }

    public class TaxonNodeStatusData{
        /**
         * @return the taxonNodeRef
         */
        public EntityReference getTaxonNodeRef() {
            return taxonNodeRef;
        }
        /**
         * @param taxonNodeRef the taxonNodeRef to set
         */
        public void setTaxonNodeRef(EntityReference taxonNodeRef) {
            this.taxonNodeRef = taxonNodeRef;
        }
        /**
         * @return the statusText
         */
        public String getStatusText() {
            return statusText;
        }
        /**
         * @param statusText the statusText to set
         */
        public void setStatusText(String statusText) {
            this.statusText = statusText;
        }
        /**
         * @return the classficationtext
         */
        public String getClassficationText() {
            return classficationtext;
        }
        /**
         * @param classficationtext the classficationtext to set
         */
        public void setClassficationText(String classficationtext) {
            this.classficationtext = classficationtext;
        }
        /**
         * @return the classificationRef
         */
        public EntityReference getClassificationRef() {
            return classificationRef;
        }
        /**
         * @param classificationRef the classificationRef to set
         */
        public void setClassificationRef(EntityReference classificationRef) {
            this.classificationRef = classificationRef;
        }
        EntityReference taxonNodeRef;
        String statusText;
        String classficationtext = null;
        EntityReference classificationRef = null;
    }

}
