package eu.etaxonomy.dataportal.pages;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.RenderedWebElement;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.FindBys;
import org.openqa.selenium.support.PageFactory;

import eu.etaxonomy.dataportal.TestConfiguration;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.selenium.JUnitWebDriverWait;

public class PortalPage {

	public static final Logger logger = Logger.getLogger(PortalPage.class);

	protected WebDriver driver;
	protected final JUnitWebDriverWait wait;
	protected URL initialUrl;

	@FindBy(id="cdm_dataportal.node")
	@CacheLookup
	private RenderedWebElement portalContent;

	@FindBy(tagName="title")
	@CacheLookup
	private RenderedWebElement title;

	//tabs primary
	@FindBys({@FindBy(id="tabs-wrapper"), @FindBy(className="primary")})
	@CacheLookup
	private RenderedWebElement primaryTabs;

	public PortalPage(WebDriver driver) throws MalformedURLException {
		this.driver = driver;
		this.initialUrl = new URL(driver.getCurrentUrl());
		this.wait = new JUnitWebDriverWait(driver, 25);

		logger.info("loading " + initialUrl);
	}

	public void goToInitialPage() {
		driver.get(initialUrl.toString());
		PageFactory.initElements(driver, this);
	}

	/**
	 * returns the string from the <code>title</code> tag.
	 * @return
	 */
	public String getTitle() {
		return title.getText();
	}

	public List<LinkElement> getPrimaryTabs(){
		List<LinkElement> tabs = new ArrayList<LinkElement>();
		List<WebElement> links = primaryTabs.findElements(By.tagName("a"));
		for(WebElement a : links) {
			RenderedWebElement renderedLink = (RenderedWebElement)a;
			if(renderedLink.isDisplayed()){
				tabs.add(new LinkElement(renderedLink));
			}
		}

		return tabs;
	}


	/**
	 * Returns the current URL string from the {@link WebDriver}
	 * @return
	 */
	public String getURL() {
		return driver.getCurrentUrl();
	}

	/**
	 * return the <code>scheme://domain:port</code> part of the initial url of this page.
	 * @return
	 */
	public String getInitialUrlBase() {
		return initialUrl.getProtocol() + "://" + initialUrl.getHost() + initialUrl.getPort();
	}

	public boolean equals(Object obj) {
		if (PortalPage.class.isAssignableFrom(obj.getClass())) {
			PortalPage page = (PortalPage) obj;
			return this.getURL().equals(page.getURL());

		} else {
			return false;
		}
	}

}
