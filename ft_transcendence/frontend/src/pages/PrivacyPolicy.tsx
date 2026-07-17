export default function PrivacyPolicy() {
  return (
    <div className="min-h-screen bg-[#f5f5f3] py-12 px-4 font-sans">
      <div className="max-w-2xl mx-auto">
        <h1 className="text-2xl font-semibold text-gray-900 mb-1">Privacy Policy</h1>
        <p className="text-[12px] text-gray-400 mb-8">Last updated: June 2026</p>

        <div className="space-y-8 text-[14px] text-gray-700 leading-relaxed">
          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">1. Information We Collect</h2>
            <p>
              We collect the following information when you use this platform: your login username,
              email address, messages sent in channels and direct messages, profile avatar, and
              general usage data such as feature interactions and session activity.
            </p>
          </section>

          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">2. How We Use Your Data</h2>
            <p>
              Your data is used solely to provide platform functionality — authentication, messaging,
              collaboration features, and profile management. Messages that invoke AI features are
              sent to the Groq API for processing. We do not sell your personal data to third parties.
            </p>
          </section>

          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">3. Data Storage</h2>
            <p>
              Your data is stored locally on our servers using PostgreSQL. It is not shared with
              external parties except when messages are sent to the Groq API for AI processing.
              Groq's own privacy policy governs that data.
            </p>
          </section>

          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">
              4. Your Rights
            </h2>
            <p>
              You can delete your account at any time from your profile
              settings. Deleting your account deactivates your profile
              and removes personally identifying information (email and
              login credentials). To preserve the integrity of other
              users' conversation history, your past messages may remain
              visible with your account shown as "deactivated."
            </p>
          </section>

          <section>
            <h2 className="text-[15px] font-semibold text-gray-900 mb-2">5. Contact</h2>
            <p>
              This platform is operated by 42 Paris students for educational purposes. For any
              privacy-related questions, please reach out through the platform's internal messaging
              or your 42 Paris intranet contact.
            </p>
          </section>
        </div>
      </div>
    </div>
  );
}
