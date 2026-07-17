import { Cat } from "lucide-react";

type Props = { onClick?: () => void };

const isPrideMonth = new Date().getMonth() === 5; // June = 5

export default function RobotCatAvatar({ onClick }: Props) {
  return (
    <div
      className={`relative w-8 h-8 rounded-full bg-indigo-50 border border-indigo-100 flex items-center justify-center shrink-0 cursor-pointer ${isPrideMonth ? "opacity-90 saturate-150" : ""}`}
      style={isPrideMonth ? {
        filter: "drop-shadow(0 0 4px rgba(255,100,200,0.4))",
      } : {}}
      onClick={onClick}
    >
      <Cat className="w-4 h-4 text-indigo-400" />
    </div>
  );
}
