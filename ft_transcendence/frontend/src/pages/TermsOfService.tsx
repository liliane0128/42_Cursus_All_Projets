export default function TermsOfService() {
  return (
    <div className="min-h-screen bg-[#f5f5f3] py-12 px-4 font-sans">
      <div className="max-w-2xl mx-auto">
        <h1 className="text-2xl font-semibold text-gray-900 mb-1">Terms of Service</h1>
        <p className="text-[12px] text-gray-400 mb-8">Last updated: June 2026</p>

        <div className="space-y-8 text-[14px] text-gray-700 leading-relaxed">
          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">1. Acceptance</h2>
            <p>
              By accessing or using this platform, you agree to be bound by these Terms of Service.
              If you do not agree, please do not use the platform.
            </p>
          </section>

          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">2. Intended Use</h2>
            <p>
              This platform is intended for 42 Paris students for collaborative work and
              communication. Use of the platform for purposes outside this scope is not permitted.
            </p>
          </section>

          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">3. Prohibited Conduct</h2>
            <p>
              The following are strictly prohibited: harassment of other users, hate speech,
              discriminatory content of any kind, and sharing other users' private information
              without their consent. Violations may result in account suspension or removal.
            </p>
          </section>

          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">4. AI Features</h2>
            <p>
              Our AI assistant, Meowlinette, is powered by Groq. By using AI features, you agree
              that your messages may be sent to Groq's API for processing. Repeated violations of
              content policies — including sending harmful, abusive, or prohibited content to the
              AI — will result in temporary suspension of AI features for your account.
            </p>
          </section>

          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">5. Disclaimer</h2>
            <p>
              This is an educational project created by 42 Paris students. The platform is provided
              as-is, without warranties of any kind, express or implied. We are not liable for any
              loss or damage arising from your use of the platform.
            </p>
          </section>

          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">6. Changes</h2>
            <p>
              We reserve the right to update these Terms of Service at any time. Continued use of
              the platform after changes are posted constitutes acceptance of the revised terms.
            </p>
          </section>
        </div>
      </div>
    </div>
  );
}
