/**
* Copyright (C) 2020 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.time.Duration;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

/**
 * @author a.kohlbecker
 * @since Aug 13, 2020
 */
public class Dynabox extends BaseElement {

    private WebDriver driver;

    private Duration timeOutInSeconds = Duration.ofSeconds(10);

    private By ajaxContentSelector;

    private LinkElement link;

    private BaseElement contentElement = null;


    /**
     * the inner content content element in {@code <div id="dynabox-(uuid)-content"><div class="dynabox-content-inner"><div class="content>}
     * which is loaded asynchronously after clicking the link element of the dynabox
     */
    public BaseElement getContentElement() {
        return contentElement;
    }

    public Dynabox(WebElement element, WebDriver driver) {
        super(element);
        this.driver = driver;

        ajaxContentSelector = By.cssSelector("#" + element.getAttribute("id") + "-content .content ");
        link = new LinkElement(element.findElement(By.tagName("a")));
    }


    public BaseElement click() {

        WebDriverWait wait = new WebDriverWait(driver, timeOutInSeconds);
        link.getElement().click();
        wait.until(ExpectedConditions.presenceOfElementLocated(ajaxContentSelector));
        contentElement = new BaseElement(getElement().findElement(ajaxContentSelector));
        return contentElement;
    }


    public Duration getTimeOutInSeconds() {
        return timeOutInSeconds;
    }


    public void setTimeOutInSeconds(Duration timeOutInSeconds) {
        this.timeOutInSeconds = timeOutInSeconds;
    }


}
