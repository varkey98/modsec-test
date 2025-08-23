#include <iostream>

#include <modsecurity/modsecurity.h>
#include <modsecurity/transaction.h>
#include <modsecurity/rules_set.h>
#include <modsecurity/rule_message.h>

void ModsecurityTest();

int main()
{
    std::cout << "Hello world!" << std::endl;
    std::cout << "Test project for modsec windows is here!\n";
    ModsecurityTest();
    return 0;
}

void ModsecurityTest()
{
    auto modsec = std::make_unique<modsecurity::ModSecurity>();

    const char main_rule_uri[] = "C:\\project\\source\\basic_rules.conf";
    auto rules = std::make_unique<modsecurity::RulesSet>();
    if (rules->loadFromUri(main_rule_uri) < 0)
    {
        std::cerr << "Problems loading the rules..." << std::endl;
        std::cerr << rules->m_parserError.str() << std::endl;
        return;
    }

    auto modsecTransaction = std::make_unique<modsecurity::Transaction>(modsec.get(), rules.get(), nullptr);
    modsecTransaction->processConnection("127.0.0.1", 12345, "127.0.0.1", 80);
    modsecTransaction->processURI(
        "https://www.modsecurity.org/test?foo=herewego",
        "GET", "1.1");

    modsecTransaction->addRequestHeader("User-Agent",
                                        "Basic ModSecurity example");
    modsecTransaction->addRequestHeader("Jacob",
                                        "matched-header");
    modsecTransaction->processRequestHeaders();
    modsecTransaction->processRequestBody();

    modsecTransaction->addResponseHeader("HTTP/1.1",
                                         "200 OK");
    modsecTransaction->processResponseHeaders(200, "HTTP 1.2");
    modsecTransaction->processResponseBody();

    modsecTransaction->processLogging();

    for (const auto &x : modsecTransaction->m_rulesMessages)
    {
        std::cout << std::to_string(x.m_rule.m_ruleId) << " " << x.m_message << std::endl;
    }
}