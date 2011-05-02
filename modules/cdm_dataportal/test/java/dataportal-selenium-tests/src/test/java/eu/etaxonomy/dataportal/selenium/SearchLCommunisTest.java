package eu.etaxonomy.dataportal.selenium;

import java.io.IOException;

import junit.framework.Assert;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.firefox.FirefoxDriver;
import org.openqa.selenium.firefox.FirefoxProfile;
import org.openqa.selenium.htmlunit.HtmlUnitDriver;
import org.openqa.selenium.ie.InternetExplorerDriver;

public class SearchLCommunisTest extends CdmDataPortalTestBase {
	
	static String baseUrl = "http://wp6-cichorieae.e-taxonomy.eu/portal/";

    @Before
    public void setUpDriver() {
    	driver = initFirefoxDriver();
    }


    @After
    public void closeDriver() {
        driver.quit();
    }

    @Test
	public void testSearchLCommunis() throws Exception {
    	driver.get(baseUrl + "?query=Lapsana+com*&search[tree]=534e190f-3339-49ba-95d9-fa27d5493e3e&q=cdm_dataportal%2Fsearch%2Ftaxon&search[pageSize]=25&search[pageNumber]=0&search[doTaxa]=1&search[doSynonyms]=1&search[doTaxaByCommonNames]=0");
    	WebElement taxonElement = driver.findElement(By.xpath("/html/body/div/div/div[2]/div[2]/div/div/div/ul/li/span[@ref='/name/f280f79f-5903-47b0-8352-53e4204c6cf1']"));
    	
    	WebElement nameElement = taxonElement.findElement(By.className("BotanicalName"));
    	Assert.assertEquals("Lapsana", nameElement.findElement(By.xpath("span[1]")).getText());
    	Assert.assertEquals("communis", nameElement.findElement(By.xpath("span[2]")).getText());
    	Assert.assertEquals("L.", nameElement.findElement(By.xpath("span[3]")).getText());
    	
    	WebElement referenceElement = taxonElement.findElement(By.className("reference"));
    	Assert.assertEquals("Sp. Pl.: 811. 1753", referenceElement.findElement((By.className("reference"))).getText());
	}


}
