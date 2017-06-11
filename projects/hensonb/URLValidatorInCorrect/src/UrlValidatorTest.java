/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import junit.framework.TestCase;
import java.util.List;
import java.util.ArrayList;



/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 */
public class UrlValidatorTest extends TestCase {

	private boolean printStatus = false;
	private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

	public UrlValidatorTest(String testName) {
		super(testName); // why is this here? what does it do? seems mysterious but important
	}

	protected void setUp() { // dont need this yet, but it might come in handy later
		// for (int index = 0; index < testPartsIndex.length - 1; index++) {
			// testPartsIndex[index] = 0;
		// }
	}


	public void testManualTest() {
		System.out.println("\"manual\" testing");
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		// lots of tests of specific hardcoded URLs, little or no pattern
		
		System.out.println("*************scheme");
		System.out.println(urlVal.isValid("http://www.amazon.com") + 	"-true");
		System.out.println(urlVal.isValid("ftp://www.amazon.com") + 	"-true");
		System.out.println(urlVal.isValid("htp://www.amazon.com") + 	"-??");
		System.out.println(urlVal.isValid("abc123xyz://www.amazon.com") + 	"-??");
		// these pass because of the ALLOW_ALL_SCHEMES option
		System.out.println(urlVal.isValid("h3t://www.amazon.com") + 	"-true");
		System.out.println(urlVal.isValid("https://www.amazon.com") + 	"-true");
		System.out.println(urlVal.isValid("www.amazon.com") + 			"-false");
		// i guess the "autocomplete with http" feature is a different code package? i thought this would be okay
		System.out.println(urlVal.isValid("://www.amazon.com") + 		"-false");
		System.out.println(urlVal.isValid("http:/www.amazon.com") + 	"-false");
		System.out.println(urlVal.isValid("http//www.amazon.com") + 	"-false");
		
		System.out.println("*************authority (domain)");
		System.out.println(urlVal.isValid("http://www.amazon.com") + 	"-true");
		System.out.println(urlVal.isValid("http://amazon.com") + 		"-true");
		System.out.println(urlVal.isValid("http://www.amazon.br") + 	"-true");
		System.out.println(urlVal.isValid("http://www.amazon.cn") + 	"-true");
		System.out.println(urlVal.isValid("http://www.amazon.eu") + 	"-true");
		System.out.println(urlVal.isValid("http://www.amazon.io") + 	"-true");
		System.out.println(urlVal.isValid("http://www.amazon.ru") + 	"-true*");
		System.out.println(urlVal.isValid("http://www.amazon.zw") + 	"-true*");
		// bug identified!
		System.out.println(urlVal.isValid("http://www.amazon.biz") + 	"-true");
		System.out.println(urlVal.isValid("http://www.amazon.info") + 	"-true");
		System.out.println(urlVal.isValid("http://.amazon.com") + 		"-false");
		System.out.println(urlVal.isValid("http://www.amazon.") + 		"-false");
		System.out.println(urlVal.isValid("http://www...com") + 		"-false");
		System.out.println(urlVal.isValid("http://www.am!?$#azon.com") +"-false");
		System.out.println(urlVal.isValid("http://www.amazon") + 		"-false");
		System.out.println(urlVal.isValid("http://amazon") + 			"-false");
		System.out.println("*************authority (IP)");
		System.out.println(urlVal.isValid("http://0.0.0.0") + 			"-true");
		System.out.println(urlVal.isValid("http://255.255.255.255") + 	"-true");
		System.out.println(urlVal.isValid("http://300.255.255.255") + 	"-false*");
		System.out.println(urlVal.isValid("http://300.300.300.300") + 	"-false*");
		// bug identified!
		System.out.println(urlVal.isValid("http://0.0.0.0.0") + 		"-false");
		System.out.println(urlVal.isValid("http://0.0.0") + 			"-false");
		System.out.println(urlVal.isValid("http://1.2.a.4") + 			"-false");
		System.out.println(urlVal.isValid("http://1.2..3.4") + 			"-false");
		
		System.out.println("*************port");
		System.out.println(urlVal.isValid("http://www.com:0") + 		"-true");
		System.out.println(urlVal.isValid("http://www.com:04") + 		"-??");
		// i dont think this should work, but then again i'm no expert on URL structure
		System.out.println(urlVal.isValid("http://www.com:65535") + 	"-true*");
		System.out.println(urlVal.isValid("http://www.com:00000") + 	"-true*");
		// bug identified!
		System.out.println(urlVal.isValid("http://www.com:65536") + 	"-false");
		System.out.println(urlVal.isValid("http://www.com:99999") + 	"-false");
		System.out.println(urlVal.isValid("http://www.com:111111") + 	"-false");
		System.out.println(urlVal.isValid("http://www.com:028") + 		"-true");
		System.out.println(urlVal.isValid("http://www.com:0028") + 		"-true*");
		// same bug
		System.out.println(urlVal.isValid("http://www.com:-1") + 		"-false");
		System.out.println(urlVal.isValid("http://www.com:x4f") + 		"-false");

		System.out.println("*************path");
		System.out.println(urlVal.isValid("http://www.com:0/asdf/qwer") + 		"-true");
		System.out.println(urlVal.isValid("http://www.com:0/asdf/qwer/") + 		"-true");
		System.out.println(urlVal.isValid("http://www.com:0/as:df/qwer/") + 	"-??");
		// i dont think this should work, but then again i'm no expert on URL structure
		System.out.println(urlVal.isValid("http://www.com:0//asdf/qwer/") + 	"-false");
		System.out.println(urlVal.isValid("http://www.com:0/asdf//qwer/") + 	"-false");
		System.out.println(urlVal.isValid("http://www.com:0/asdf/qwer//") + 	"-false");
		System.out.println(urlVal.isValid("http://www.com:0/asdf...qwer/") + 	"-false");
		System.out.println(urlVal.isValid("http://www.com:0/..") + 				"-false");
		System.out.println(urlVal.isValid("http://www.com:0/../") + 			"-false");
		System.out.println(urlVal.isValid("http://www.com:0/asdf/../qwer/") + 	"-true");
		System.out.println(urlVal.isValid("http://www.com:0/../asdf/qwer/") + 	"-??");
		// again, i dont think this should work, but then again i'm no expert on URL structure
		System.out.println(urlVal.isValid("http://www.com:0/asdf/$24/qwer/") + 	"-true");
		
		System.out.println("*************query");
		System.out.println(urlVal.isValid("http://www.com:0/asdf/") + 						"-true");
		System.out.println(urlVal.isValid("http://www.com:0/asdf/?ilnj^&*(!&%^^[[[") + 		"-true*");
		// pretty sure this should be not okay, but on the clean version it passes
		System.out.println(urlVal.isValid("http://www.com:0/asdf/?") + 						"-true*");
		System.out.println(urlVal.isValid("http://www.com:0/asdf/?action=view") + 			"-true*");
		System.out.println(urlVal.isValid("http://www.com:0/asdf/?action=edit&mode=up") +	"-true*");
		// bug identified!
		
		}


	public void testYourFirstPartition() {
		// some more intelligent testing choices
		// the "manual testing" section is pretty intelligent and breaks up the input domain well
	}

	public void testYourSecondPartition() {
		// some more intelligent testing choices
		// the "manual testing" section is pretty intelligent and breaks up the input domain well
	}


	public void testIsValid() {
		// basically the same as the test we were originally provided...
		//     BUT instead of testing everything with everything, have one "base URL" and test each
		//     section's possibilities with that base URL
		// based in the assumption that nothing is wrong with the highest-level REGEX parsing, and that
		//     there's no wonky interactions between URL segments that we need to test for... AKA can
		//     safely test only one at a time
		// tests mostly the same parts as in the "manual testing" section
		
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		
		// 5 lists of segments to test , type ResultPair[] testUrlScheme
		ResultPair[] testUrlScheme = {	new ResultPair("http://", 	true),
										new ResultPair("ftp://", 	true),
										new ResultPair("https://", 	true),
										new ResultPair("htp://", 	true), // these only valid because ALLOW_ALL_SCHEMES option
										new ResultPair("abc123xyz://", true), // these only valid because ALLOW_ALL_SCHEMES option
										new ResultPair("h3t://", 	true), // these only valid because ALLOW_ALL_SCHEMES option
										new ResultPair("s://", 		true), // these only valid because ALLOW_ALL_SCHEMES option
										new ResultPair("", 			false),
										new ResultPair("://", 		false),
										new ResultPair("http:/", 	false),
										new ResultPair("h&(ttp://", false), // ?
										new ResultPair("http//", 	false)
		};
		ResultPair[] testUrlAuth = {	new ResultPair("www.amazon.com", true),
										new ResultPair("amazon.com", 	true),
										new ResultPair("www.amazon.br", true),
										new ResultPair("www.amazon.cn", true),
										new ResultPair("www.amazon.eu", true),
										new ResultPair("www.amazon.io", true),
										new ResultPair("www.amazon.ru", true), // bug
										new ResultPair("www.amazon.zw", true), // bug
										new ResultPair("www.amazon.biz", true),
										new ResultPair("www.amazon.info", true),
										new ResultPair("www.amazon.gov", true),
										new ResultPair(".amazon.gov", 	false),
										new ResultPair("www.amazon.", 	false),
										new ResultPair("www.amazon", 	false),
										new ResultPair("www....com", 	false),
										new ResultPair("www.am!?$#azon.com", false),
										new ResultPair("amazon", 		false),
										
										new ResultPair("0.0.0.0", 		true),
										new ResultPair("1.2.3.4", 		true),
										new ResultPair("255.255.255.255", true),
										new ResultPair("300.255.255.255", false), // bug
										new ResultPair("300.300.300.300", false), // bug
										new ResultPair("3010.3020.3030.3040", false),
										new ResultPair("0.0.0.0.0", 	false),
										new ResultPair("0.0.0", 		false),
										new ResultPair("1.2.a.4", 		false),
										new ResultPair("1.2..3.4", 		false)
		};
		ResultPair[] testUrlPort = {	new ResultPair(":0", 	true),
										new ResultPair(":07", 	true), // i dont think this should work, but i'm no expert
										new ResultPair("",	 	true),
										new ResultPair(":1", 	true),
										new ResultPair(":12", 	true),
										new ResultPair(":123", 	true),
										new ResultPair(":1234", true), // bug
										new ResultPair(":12345", true), // bug
										new ResultPair(":65535", true), // bug
										new ResultPair(":65536", false),
										new ResultPair(":99999", false),
										new ResultPair(":123456", false),
										new ResultPair(":255", 	true),
										new ResultPair("::411", false),
										new ResultPair(":-1", 	false),
										new ResultPair(":x4f", 	false)
		};
		ResultPair[] testUrlPath = {	new ResultPair("/asdf/qwer", 	true),
										new ResultPair("/asdf/qwer/", 	true),
										new ResultPair("/as:df/qwer/", 	true), // i dont think this should work, but i'm no expert
										new ResultPair("//asdf/qwer/", 	false),
										new ResultPair("/asdf//qwer/", 	false),
										new ResultPair("/asdf/qwer//", 	false),
										new ResultPair("/asdf...qwer/", false),
										new ResultPair("/asdf.qwer/", 	true),
										new ResultPair("/..", 			false),
										new ResultPair("/../", 			false),
										new ResultPair("/asdf/../qwer/", true), // i dont think this should work, but i'm no expert
										new ResultPair("/../asdf/qwer/", true), // i dont think this should work, but i'm no expert
										new ResultPair("/asdf/$24/qwer/", true), // i dont think this should work, but i'm no expert
										new ResultPair("/asdf/2$4/qwer/", true), // ?
										new ResultPair("3ht://", 		false)
		};
		ResultPair[] testUrlQuery = {	new ResultPair("", 					true),
										new ResultPair("?", 				true), // bug // i dont think this should work, but it works on the clean version
										new ResultPair("?action=view", 		true), // bug
										new ResultPair("?action=edit&mode=up", true), // bug
										new ResultPair("????", 				true), // bug // i dont think this should work, but it works on the clean version
										new ResultPair("?ilnj^&*(!&%^^[[[", true)  // bug // i dont think this should work, but it works on the clean version
		};
		// combine them into Object[] testParts
		Object[] testParts = {testUrlScheme, testUrlAuth, testUrlPort, testUrlPath, testUrlQuery};
		// set up default URL parts
		String normScheme = "http://";
		String normAuth   = "www.google.com";
		String normPort   = ":0";		// this can be empty
		String normPath   = "/qwerty";	// this can be empty
		String normQuery  = "";			// this should be empty
		
		// variables used by the loop below:
		ResultPair[] activeList = {};
		ResultPair segment;
		String UrlTestMe;
		int whichList = 0;
		int listIdx = 0;
		boolean result = true;
		List<String> failures = new ArrayList<String>();
		int loops = 0;
		
		for (whichList = 0; whichList < 5; whichList += 1) {
			activeList = (ResultPair[]) testParts[whichList]; // which list is it looking at, and cast to proper type
			for (listIdx = 0; listIdx < activeList.length; listIdx += 1) {
				segment = activeList[listIdx];
				UrlTestMe = (whichList==0 ? segment.item : normScheme) + 
				            (whichList==1 ? segment.item : normAuth) + 
				            (whichList==2 ? segment.item : normPort) + 
				            (whichList==3 ? segment.item : normPath) + 
				            (whichList==4 ? segment.item : normQuery);
				// System.out.println(UrlTestMe);					// debug
				// System.out.println(whichList + " " + listIdx);	// debug
				result = urlVal.isValid(UrlTestMe);
				loops += 1;
				if(result != segment.valid) {
					// add to the failures list, instead of using an assert and simply dying
					failures.add(whichList + "," + listIdx + "=\" " + UrlTestMe + " \"");
				}
			}
		}
		// print out results and statistics
		System.out.println("failure rate:      " + failures.size() + " fail / " + loops + " total");
		if(failures.size() > 0) {
			System.out.println("********** list of URLs that were incorrectly judged: *************");
			System.out.println("whichList,listIdx=\" URL \"");
			for (String item : failures) {		// list the URLs which were incorrectly judged
				System.out.println(item);
			}
		}
	}
	
	public void testAnyOtherUnitTest() {
		// once bugs are found and understood, create 3 unit tests/functions that highlight those bugs
		// i think the function testIsValid() produces results clear enough that specific unit test functions
		//     are not needed.
	}


	public static void main(String[] argv) {
		
		System.out.println("start!");
		UrlValidatorTest fct = new UrlValidatorTest("url test");
		fct.setUp(); // dont actually have this defined yet, might be useful tho
		
		fct.testManualTest();
		fct.testYourFirstPartition();
		fct.testYourSecondPartition();
		
		fct.testIsValid();
		fct.testAnyOtherUnitTest();
		
		System.out.println("done!");
	}

}
