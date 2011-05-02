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

public class SearchLCommunis  {
	
	WebDriver driver;
	
	static String baseUrl = "http://wp6-cichorieae.e-taxonomy.eu/portal/";

    @Before
    public void setUpDriver() {
    	
    	driver = initFirefoxDriver();
    }

	private WebDriver initChromeDriver() {
		System.setProperty("webdriver.chrome.bin", "C:\\Dokumente und Einstellungen\\a.kohlbecker.BGBM\\Lokale Einstellungen\\Anwendungsdaten\\Google\\Chrome\\Application\\chrome.exe");
		return new ChromeDriver();
	}
	
	private WebDriver initInternetExplorerDriver() {
		return new InternetExplorerDriver();
	}
	
	/**
	 * See http://code.google.com/p/selenium/wiki/FirefoxDriverInternals
	 * @return
	 */
	private WebDriver initFirefoxDriver() {
		FirefoxProfile firefoxProfile = new FirefoxProfile();
    	try {
			firefoxProfile.addExtension(SearchLCommunis.class, "/org/mozilla/addons/add_on_compatibility_reporter-0.8.3-fx+tb+sm.xpi");
			firefoxProfile.setPreference("extensions.acr.firstrun", false);
			firefoxProfile.setPreference("extensions.enabledAddons", "fxdriver@googlecode.com,compatibility@addons.mozilla.org:0.8.3,fxdriver@googlecode.com:0.9.7376,{CAFEEFAC-0016-0000-0024-ABCDEFFEDCBA}:6.0.24,{20a82645-c095-46ed-80e3-08825760534b}:0.0.0,meetinglauncher@iconf.net:4.10.12.316,jqs@sun.com:1.0,{972ce4c6-7e08-4474-a285-3208198ce6fd}:4.0");
			firefoxProfile.setPreference("extensions.checkCompatibility", false);
			firefoxProfile.setPreference("extensions.checkCompatibility.4.0", false);
			firefoxProfile.setPreference("extensions.checkCompatibility.4.1", false);
			
    	} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.exit(-1);
		}
        return new FirefoxDriver(firefoxProfile);
	}

    @After
    public void closeDriver() {
        driver.quit();
    }

    @Test
	public void testSearchLCommunis() throws Exception {
    	driver.get(baseUrl + "?query=Lapsana+com*&search%5Btree%5D=534e190f-3339-49ba-95d9-fa27d5493e3e&q=cdm_dataportal%2Fsearch%2Ftaxon&search%5BpageSize%5D=25&search%5BdoTaxa%5D=1&search%5BdoSynonyms%5D=1&search%5BdoTaxaByCommonNames%5D=0");
    	WebElement taxonElement = driver.findElement(By.xpath("/html/body/div/div/div[2]/div[2]/div/div/div/ul/li/span[@ref='/name/f280f79f-5903-47b0-8352-53e4204c6cf1']"));
    	
    	WebElement nameElement = taxonElement.findElement(By.className("BotanicalName"));
    	Assert.assertEquals("Lapsana", nameElement.findElement(By.xpath("span[0]")).getText());
    	Assert.assertEquals("communis", nameElement.findElement(By.xpath("span[1]")).getText());
    	Assert.assertEquals("L.", nameElement.findElement(By.xpath("span[2]")).getText());
    	
    	WebElement referenceElement = taxonElement.findElement(By.className("BotanicalName"));
    	Assert.assertEquals("Sp. Pl.: 811. 1753", referenceElement.findElement((By.className("reference"))).getText());
	}


}
