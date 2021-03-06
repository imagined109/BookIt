// A simple demo of JNI interface to implement SIFT detection for Android application using nonfree module in OpenCV4Android.
// Created by Guohui Wang 
// Email: robertwgh_at_gmail_com
// Data: 2/26/2014

#include <jni.h>
#include <android/log.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define  LOG_TAG    "BookIt"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)






const char* fileNames[] = {
	"0262182629", "http://www.amazon.com/Processing-Programming-Handbook-Designers-Artists/dp/0262182629%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0262182629", 
	"0465018475", "http://www.amazon.com/Surfaces-Essences-Analogy-Fuel-Thinking/dp/0465018475%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0465018475", 
	"0387310738", "http://www.amazon.com/Pattern-Recognition-Learning-Information-Statistics/dp/0387310738%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0387310738", 
	"1581604947", "http://www.amazon.com/The-Ultimate-Sniper-Advanced-Training/dp/1581604947%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1581604947", 
	"0385495323", "http://www.amazon.com/The-Code-Book-Science-Cryptography/dp/0385495323%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0385495323", 
	"0262013193", "http://www.amazon.com/Probabilistic-Graphical-Models-Principles-Computation/dp/0262013193%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0262013193", 
	"1591846900", "http://www.amazon.com/Big-Bang-Disruption-Devastating-Innovation/dp/1591846900%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1591846900", 
	"0321767535", "http://www.amazon.com/Things-Designer-People-Voices-Matter/dp/0321767535%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0321767535", 
	"1133958494", "http://www.amazon.com/Sensation-Perception-CourseMate-Printed-Access/dp/1133958494%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1133958494", 
	"0743222091", "http://www.amazon.com/Diffusion-Innovations-Edition-Everett-Rogers/dp/0743222091%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0743222091", 
	"0140067485", "http://www.amazon.com/The-Puzzle-Palace-Intelligence-Organization/dp/0140067485%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0140067485", 
	"0471603856", "http://www.amazon.com/Seeing-Light-Optics-Photography-Holography/dp/0471603856%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0471603856", 
	"0321605780", "http://www.amazon.com/Agile-Product-Management-Scrum-Addison-Wesley/dp/0321605780%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0321605780", 
	"0761917527", "http://www.amazon.com/Images-Organization-The-Executive-Edition/dp/0761917527%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0761917527", 
	"0679735666", "http://www.amazon.com/Natural-History-Senses-Diane-Ackerman/dp/0679735666%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0679735666", 
	"4805311290", "http://www.amazon.com/Geek-Japan-Discovering-Manga-Ceremony/dp/4805311290%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D4805311290", 
	"0470665769", "http://www.amazon.com/Interaction-Design-Beyond-Human-Computer/dp/0470665769%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0470665769", 
	"076454280X", "http://www.amazon.com/The-Art-Deception-Controlling-Security/dp/076454280X%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D076454280X", 
	"1400068711", "http://www.amazon.com/The-Age-Insight-Understand-Unconscious/dp/1400068711%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1400068711", 
	"1118549368", "http://www.amazon.com/Exploring-Arduino-Techniques-Engineering-Wizardry/dp/1118549368%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1118549368", 
	"0765334380", "http://www.amazon.com/Pillar-Sky-William-R-Forstchen/dp/0765334380%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0765334380", 
	"1452202990", "http://www.amazon.com/Mass-Communication-Living-Opinion-Edition/dp/1452202990%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1452202990", 
	"1556433417", "http://www.amazon.com/Relearning-See-Improve-Eyesight-Naturally/dp/1556433417%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1556433417", 
	"1601631898", "http://www.amazon.com/The-Big-Book-Barbara-Mitchell/dp/1601631898%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1601631898", 
	"0123736021", "http://www.amazon.com/Learning-Processing-Beginners-Programming-Interaction/dp/0123736021%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0123736021", 
	"1621365328", "http://www.amazon.com/Spiritual-Avalanche-Teachings-Destroy-Millions/dp/1621365328%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1621365328", 
	"0321898656", "http://www.amazon.com/Data-Just-Right-Introduction-Addison-Wesley/dp/0321898656%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0321898656", 
	"0451417704", "http://www.amazon.com/Kill-Decision-Daniel-Suarez/dp/0451417704%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0451417704", 
	"1595588671", "http://www.amazon.com/Digital-Disconnect-Capitalism-Internet-Democracy/dp/1595588671%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1595588671", 
	"1477809783", "http://www.amazon.com/Terms-Enlistment-Frontlines-Marko-Kloos/dp/1477809783%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1477809783", 
	"0465026567", "http://www.amazon.com/G%C3%B6del-Escher-Bach-Eternal-Golden/dp/0465026567%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0465026567", 
	"0393334775", "http://www.amazon.com/How-Mind-Works-Steven-Pinker/dp/0393334775%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0393334775", 
	"1118795482", "http://www.amazon.com/Raspberry-User-Guide-Eben-Upton/dp/1118795482%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1118795482", 
	"B00BJEA5J2", "http://www.amazon.com/Shades-Children-Garth-Nix/dp/B00BJEA5J2%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3DB00BJEA5J2", 
	"0062128256", "http://www.amazon.com/Haunted-Empire-Apple-After-Steve/dp/0062128256%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0062128256", 
	"0123814642", "http://www.amazon.com/Information-Visualization-Third-Edition-Technologies/dp/0123814642%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0123814642", 
	"0062120999", "http://www.amazon.com/Great-Choice-Uncertainty-Luck-Why-Despite/dp/0062120999%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0062120999", 
	"0307887898", "http://www.amazon.com/The-Lean-Startup-Entrepreneurs-Continuous/dp/0307887898%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0307887898", 
	"1610393090", "http://www.amazon.com/Unleashing-Second-American-Century-Dominance/dp/1610393090%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1610393090", 
	"0982634811", "http://www.amazon.com/Interdisciplinary-Interaction-Design-Interactive-Experiences/dp/0982634811%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0982634811", 
	"0307472256", "http://www.amazon.com/Subliminal-Unconscious-Rules-Behavior-Vintage/dp/0307472256%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0307472256", 
	"0123814790", "http://www.amazon.com/Data-Mining-Concepts-Techniques-Management/dp/0123814790%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0123814790", 
	"0307588696", "http://www.amazon.com/Kingpin-Hacker-Billion-Dollar-Cybercrime-Underground/dp/0307588696%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0307588696", 
	"0124166725", "http://www.amazon.com/Cyber-Warfare-Second-Edition-Practitioners/dp/0124166725%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0124166725", 
	"1934938750", "http://www.amazon.com/Eye-Yoga-How-You-Think/dp/1934938750%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1934938750", 
	"0470531290", "http://www.amazon.com/Contemporary-Business-Louis-E-Boone/dp/0470531290%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0470531290", 
	"0452298776", "http://www.amazon.com/100-Diagrams-That-Changed-World/dp/0452298776%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0452298776", 
	"1449356435", "http://www.amazon.com/Zero-Maker-Learn-Enough-Anything/dp/1449356435%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1449356435", 
	"1422102823", "http://www.amazon.com/The-Medici-Effect-Elephants-Innovation/dp/1422102823%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1422102823", 
	"1599791773", "http://www.amazon.com/Commanding-Your-Morning-Unleash-Power/dp/1599791773%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1599791773", 
	"038553082X", "http://www.amazon.com/The-Future-Mind-Scientific-Understand/dp/038553082X%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D038553082X", 
	"0324302592", "http://www.amazon.com/Management-Challenges-Tomorrows-InfoTrac-1-Semester/dp/0324302592%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0324302592", 
	"0465046746", "http://www.amazon.com/Mindstorms-Children-Computers-Powerful-Ideas/dp/0465046746%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0465046746", 
	"007174875X", "http://www.amazon.com/The-Innovation-Secrets-Steve-Jobs/dp/007174875X%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D007174875X", 
	"0071750363", "http://www.amazon.com/Robot-Builders-Bonanza-4th-Edition/dp/0071750363%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0071750363", 
	"1119998956", "http://www.amazon.com/Design-Hackers-Reverse-Engineering-Beauty/dp/1119998956%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1119998956", 
	"0385517254", "http://www.amazon.com/The-Fifth-Discipline-Practice-Organization/dp/0385517254%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0385517254", 
	"0840033559", "http://www.amazon.com/Cognitive-Psychology-Connecting-Research-Experience/dp/0840033559%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0840033559", 
	"0195321057", "http://www.amazon.com/How-Read-Film-Movies-Beyond/dp/0195321057%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0195321057", 
	"0495601497", "http://www.amazon.com/Sensation-Perception-E-Bruce-Goldstein/dp/0495601497%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0495601497", 
	"012375030X", "http://www.amazon.com/Designing-Mind-Simple-Understanding-Interface/dp/012375030X%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D012375030X", 
	"1449388396", "http://www.amazon.com/Hackers-Computer-Revolution-Anniversary-Edition/dp/1449388396%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1449388396", 
	"0393239357", "http://www.amazon.com/The-Second-Machine-Age-Technologies/dp/0393239357%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0393239357", 
	"0062093029", "http://www.amazon.com/Vortex-Insignia-S-J-Kincaid/dp/0062093029%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0062093029", 
	"0071413774", "http://www.amazon.com/Finance-Non-Financial-Managers-Briefcase-Series/dp/0071413774%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0071413774", 
	"1591843065", "http://www.amazon.com/The-Back-Napkin-Expanded-Edition/dp/1591843065%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1591843065", 
	"0312622376", "http://www.amazon.com/Our-Final-Invention-Artificial-Intelligence/dp/0312622376%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0312622376", 
	"0062093002", "http://www.amazon.com/Insignia-S-J-Kincaid/dp/0062093002%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0062093002", 
	"0521314194", "http://www.amazon.com/Human-Error-James-Reason/dp/0521314194%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0521314194", 
	"048624864X", "http://www.amazon.com/Introduction-Artificial-Intelligence-Enlarged-Mathematics/dp/048624864X%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D048624864X", 
	"B0090PX85A", "http://www.amazon.com/Wheel-of-Fortune/dp/B0090PX85A%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3DB0090PX85A", 
	"1118291980", "http://www.amazon.com/Contemporary-Business-Louis-E-Boone/dp/1118291980%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1118291980", 
	"0525953736", "http://www.amazon.com/Average-Is-Over-Powering-Stagnation/dp/0525953736%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0525953736", 
	"1416537805", "http://www.amazon.com/Dragon-Dirk-Adventure-Clive-Cussler/dp/1416537805%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1416537805", 
	"0393330435", "http://www.amazon.com/iWoz-Computer-Invented-Personal-Co-Founded/dp/0393330435%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0393330435", 
	"0544002695", "http://www.amazon.com/Big-Data-Revolution-Transform-Think/dp/0544002695%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0544002695", 
	"1451648537", "http://www.amazon.com/Steve-Jobs-Walter-Isaacson/dp/1451648537%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1451648537", 
	"1598571966", "http://www.amazon.com/Augmentative-Alternative-Communication-Supporting-Children/dp/1598571966%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1598571966", 
	"0393048136", "http://www.amazon.com/The-New-Thing-Silicon-Valley/dp/0393048136%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0393048136", 
	"0060008768", "http://www.amazon.com/The-Difference-Between-Larry-Ellison/dp/0060008768%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0060008768", 
	"0688177883", "http://www.amazon.com/The-Scientist-Crib-Early-Learning/dp/0688177883%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0688177883", 
	"1426208081", "http://www.amazon.com/The-Science-Book-Everything-About/dp/1426208081%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1426208081", 
	"0385484992", "http://www.amazon.com/Visions-Science-Will-Revolutionize-Century/dp/0385484992%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0385484992", 
	"1118717058", "http://www.amazon.com/Learning-Python-Raspberry-Alex-Bradbury/dp/1118717058%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1118717058", 
	"0201633612", "http://www.amazon.com/Design-Patterns-Elements-Reusable-Object-Oriented/dp/0201633612%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0201633612", 
	"1782161406", "http://www.amazon.com/Building-Machine-Learning-Systems-Python/dp/1782161406%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1782161406", 
	"1137278463", "http://www.amazon.com/Zero-Marginal-Cost-Society-Collaborative/dp/1137278463%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1137278463", 
	"1416609407", "http://www.amazon.com/Curriculum-21-Essential-Education-Changing/dp/1416609407%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1416609407", 
	"0071704426", "http://www.amazon.com/Mechanisms-Mechanical-Devices-Sourcebook-Edition/dp/0071704426%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0071704426", 
	"0262134748", "http://www.amazon.com/Designing-Interactions-Bill-Moggridge/dp/0262134748%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0262134748", 
	"0307390993", "http://www.amazon.com/The-Master-Switch-Information-Empires/dp/0307390993%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0307390993", 
	"0756637589", "http://www.amazon.com/Merriam-Webster-Childrens-Dictionary-DK-Publishing/dp/0756637589%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0756637589", 
	"0671657135", "http://www.amazon.com/The-Society-Mind-Marvin-Minsky/dp/0671657135%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0671657135", 
	"0262018020", "http://www.amazon.com/Machine-Learning-Probabilistic-Perspective-Computation/dp/0262018020%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0262018020", 
	"1449344216", "http://www.amazon.com/Getting-Started-Raspberry-Pi-Make/dp/1449344216%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1449344216", 
	"1107422221", "http://www.amazon.com/Machine-Learning-Science-Algorithms-Sense/dp/1107422221%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1107422221", 
	"1449307078", "http://www.amazon.com/Making-Things-See-Processing-MakerBot/dp/1449307078%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1449307078", 
	"0307887189", "http://www.amazon.com/Dark-Pools-Machine-Traders-Rigging/dp/0307887189%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0307887189", 
	"0385260946", "http://www.amazon.com/The-Fifth-Discipline-Peter-Senge/dp/0385260946%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D0385260946", 
	"1451659253", "http://www.amazon.com/Inside-Box-Creativity-Breakthrough-Results/dp/1451659253%3FSubscriptionId%3DAKIAJMFW6F37ROWP7LQA%26tag%3Dwwwbarkanidoc-20%26linkCode%3Dxm2%26camp%3D2025%26creative%3D165953%26creativeASIN%3D1451659253", 
	"B000PC0S7I", "http://www.amazon.com/gp/reader/B000PC0S7I"
};
const char * basePath = "/sdcard/Documents/Books/";
void buildDictionary();

struct feature {
	Mat descriptor;  
	vector<KeyPoint> keypoints;
	};
	
	

typedef unsigned char uchar;


int build_sift_xml();
std::string find_matches_sift(Mat& image,Mat& results);
feature getFeature(Mat image);


// OLD FUNCTIONS
int run_demo();
int run_sift(Mat& image,Mat& results);
std::string find_matches(Mat& image,Mat& results);
int build_Dictionary();
int buildDescriptorMat();

extern "C" {
    JNIEXPORT jstring JNICALL Java_com_example_bookit_NonfreeJNILib_runSift(JNIEnv * env, jobject obj, jlong addrInputMat, jlong addrOutputMat);//, jobjectArray fileNameArray);
};

JNIEXPORT jstring JNICALL Java_com_example_bookit_NonfreeJNILib_runSift(JNIEnv * env, jobject obj,jlong addrInputMat, jlong addrOutputMat)//, jobjectArray fileNameArray)
{
LOGI("Test! \n");
	// Load the mat from address
	Mat& inImg = *(Mat*)addrInputMat;
	Mat& outImg = *(Mat*)addrOutputMat;
	
	
	//LOGI("Start building sifts! \n");
	//build_sift_xml();
	//LOGI("Done building sifts! \n");


	
	
	std::string Link;
	LOGI("Start find_matches! \n");
	Link = find_matches_sift(inImg,outImg);
	LOGI("End find_matches! \n");
	
	return env->NewStringUTF(Link.c_str());
	
}

std::string find_matches_sift(Mat& image,Mat& results){
		
	// Get the sift of the input image.
	feature sceneFeatures;
	sceneFeatures = getFeature(image);
	LOGI("Detected %d keypoints\n", (int) sceneFeatures.keypoints.size());

	// We now look for a match between the scene and the different sifts calculated.
    FileStorage fsCovers("/sdcard/Documents/bookSifts.xml", FileStorage::READ);

	// Initialize the matcher
	vector< vector<DMatch> > matches;
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");

	std::vector< DMatch > good_matches; // The vector which stores good matches.
	std::string FinalLink; // The link which will be returned.
	int numGoodMatch = 0;
		
	// For homography
	std::vector<Point2f> cover_corners(4);
	std::vector<Point2f> scene_corners(4);
	std::vector<Point2f> keypoint_points;
	std::vector<Point2f> cover;
	std::vector<Point2f> scene;
	
	// Load the File Node
	FileNode coverDescriptors = fsCovers["descriptors"];
	FileNodeIterator it = coverDescriptors.begin(), it_end = coverDescriptors.end();
	int idx = 0;
	
	// Iterate over the different covers (and their corresponding descriptors, links, keypoints, etc.).
	for( ; it != it_end; ++it, idx++ )
	{
		
		// Clear previous vectors.
		Mat desCover;
		cover_corners.clear();
		keypoint_points.clear();
		good_matches.clear();
		
		// Get the descriptor from this book cover.
		(*it)["Descriptor"] >> desCover;
		// Calculate the matches using the flann based matcher.
		// matchCovers.match(desCover, sceneFeatures.descriptor, matches );
		 matcher->knnMatch( desCover, sceneFeatures.descriptor, matches, 2 );

		// Calculate the best matches using Lowe's ratio
		for (int i = 0; i < matches.size(); ++i)
		{
			const float ratio = 0.8; // As in Lowe's paper; can be tuned
			if (matches[i][0].distance < ratio * matches[i][1].distance)
			{
				good_matches.push_back(matches[i][0]);
			}
		}

		// If this is the best one thus far, update the link, keypoints, and corners
		if (good_matches.size() > numGoodMatch)
		{
			
			numGoodMatch = good_matches.size();
			(*it)["LINK"] >> FinalLink;
			
			// Get the corners of the matched cover.
			read((*it)["cover_corners"],cover_corners);
			
			// Get the keypoint points of the match.
			(*it)["KeypointPT"] >> keypoint_points;
		}
		
		// Release the memory
		desCover.release();
	}
	fsCovers.release();
	
	//-- Localize the book cover --//

    //-- Get the point locations of the keypoints from good matches	--//
	for( int i = 0; i < good_matches.size(); i++ )
	{
    cover.push_back( keypoint_points[ good_matches[i].queryIdx ]);
    scene.push_back( sceneFeatures.keypoints[ good_matches[i].trainIdx ].pt ); // the scene
	}
	
	// Show keypoints in the result image image.
	// Scalar keypointColor = Scalar(255, 0, 0);
	// drawKeypoints(image, sceneFeatures.keypoints, results, keypointColor, DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	
	// Copy the image to the results.
	results = image;
	
	//-- Compute Homography using RANSAC --//
	Mat H = findHomography( cover, scene, CV_RANSAC );
	perspectiveTransform( cover_corners, scene_corners, H);

  //-- Draw lines between the corners (the mapped object in the scene - image_2 ) --//  
	line( results, scene_corners[0], scene_corners[1], Scalar(0, 255, 0), 4 );
	line( results, scene_corners[1], scene_corners[2], Scalar( 0, 255, 0), 4 );
	line( results, scene_corners[2], scene_corners[3], Scalar( 0, 255, 0), 4 );
	line( results, scene_corners[3], scene_corners[0], Scalar( 0, 255, 0), 4 );

	
	// Save the results image
	// const char * imgOutFile = "/sdcard/Documents/img1_result.jpg";
	// imwrite(imgOutFile, results);
	
	return FinalLink;
	
}


int build_sift_xml(){
// This function is here just in case we want to rebuild the xml on the phone for some reason.  

	// Initialize some variables.
	char * filename = new char[100];
	char * ASIN = new char[128];
	char * LINK = new char[512];
	Mat image; //to store the current input image
	std::vector<Point2f> keypoint_points;  
	std::vector<Point2f> cover_corners(4);
	
	// Set the file to write to.
	FileStorage fs("/sdcard/Documents/bookSifts.xml", FileStorage::WRITE);
	
	fs << "descriptors" << "[";
	
	// For each book cover.
	for(int i = 0; i < 202; i += 2)
	{
		// Clear previous values.
		keypoint_points.clear();
		cover_corners.clear();
		
		// Load the image.
		sprintf(filename, "%s%s.jpg", basePath, fileNames[i]);
		LOGI("%s%s.jpg", basePath, fileNames[i]);
		image = imread(filename);//, CV_LOAD_IMAGE_GRAYSCALE);    
		
		// Calculate the keypoints and descriptor.
		feature coverFeature;
		coverFeature = getFeature(image);
		
		// Fill the vector of keypoint points (these will be stored in the xml instead of the whole keypoint).
		for (int idx=0; idx < (int) coverFeature.keypoints.size(); idx++){
			keypoint_points.push_back(coverFeature.keypoints[idx].pt);
		}
		
		// Get the corners of the cover (this is used in homography).
		cover_corners[0] = cvPoint(0,0); cover_corners[1] = cvPoint( image.cols, 0 );
		cover_corners[2] = cvPoint( image.cols, image.rows ); cover_corners[3] = cvPoint( 0, image.rows );		
		
		//create the name and descriptor tags
		strcpy(ASIN, fileNames[i]);
		strcpy(LINK, fileNames[i+1]);
		
		// Write the current bowDescriptor to the yml
		fs << "{:" << "ASIN" << ASIN << "LINK" << LINK << "cover_corners"<<cover_corners<<"KeypointPT" << keypoint_points << "Descriptor" << coverFeature.descriptor << "}";

	}
	fs << "]";
	fs.release();
}

feature getFeature(Mat image){
	// This function calculates the feature using some Detector and Extractor.  
	// It is done this way for easy swapping of descriptor/extractors.

	// int minHessian = 400;
	// SurfFeatureDetector detector(minHessian);
	// OrbFeatureDetector detector;
	// SiftDescriptorExtractor extractor;
	// // detector= new cv::OrbFeatureDetector();
	// // extractor = new cv::SiftDescriptorExtractor();
	  

	feature features;

	// Create a SIFT feature detector.
	SiftFeatureDetector detector;
	
	// Get the keypoints 
	detector.detect(image, features.keypoints);
	
	// Get the descriptor.
	detector.compute(image, features.keypoints, features.descriptor);
	// extractor.compute(image,features.keypoints,features.descriptor);
	return features;
}